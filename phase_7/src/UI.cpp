#include "UI.h"
#include "Service.h"
#include <iostream>
#include <limits>

void ConsoleUI::run() {
    AuthService auth(db);
    CourseService courseService(db);
    AssignmentService assignService(db);

    // Seed default baseline accounts and a course if running on empty database
    if (!auth.login("admin", "Admin123!").has_value()) {
        auth.registerUser("admin", "Admin123!", UserRole::ADMIN);
        auth.registerUser("lecturer1", "Lecturer1!", UserRole::LECTURER);
        auth.registerUser("hod1", "Hod1!", UserRole::HOD);
        auth.registerUser("student1", "Student1!", UserRole::STUDENT);

        auto lecturerOpt = auth.login("lecturer1", "Lecturer1!");
        auto studentOpt = auth.login("student1", "Student1!");

        if (lecturerOpt && studentOpt) {
            courseService.createCourse("CS4433", "Software Engineering", lecturerOpt->userId);
            auto courses = courseService.listAllCourses();
            if (!courses.empty()) {
                courseService.enrollStudentInCourse(courses[0].courseId, studentOpt->userId);
                assignService.createAssignment(courses[0].courseId, "C++ Assignment 1", "2026-11-30 23:59:59", 100.0);
            }
        }
    }

    std::cout << "=======================================================\n";
    std::cout << "=== Vertex LMS v1.0 (Course Structuring Enabled) ===\n";
    std::cout << "=======================================================\n";

    while (true) {
        std::string username, password;
        std::cout << "\n[LOGIN] Enter Username (or type 'exit' to quit): ";
        std::cin >> username;
        if (username == "exit") break;
        std::cout << "[LOGIN] Enter Password: ";
        std::cin >> password;

        auto userOpt = auth.login(username, password);
        if (!userOpt) {
            std::cout << "ERROR: Invalid username or password. Please try again.\n";
            continue;
        }

        switch (userOpt->role) {
            case UserRole::STUDENT: handleStudentMenu(*userOpt); break;
            case UserRole::LECTURER: handleLecturerMenu(*userOpt); break;
            case UserRole::HOD: handleHodMenu(*userOpt); break;
            case UserRole::ADMIN: handleAdminMenu(*userOpt); break;
        }
    }
}

void ConsoleUI::handleStudentMenu(const User& user) {
    CourseService courseService(db);
    AssignmentService assignService(db);

    while (true) {
        std::cout << "\n--- STUDENT MENU (User: " << user.username << ") ---\n";
        std::cout << "1. View Enrolled Courses\n";
        std::cout << "2. View Assignments & Submit Solution File\n";
        std::cout << "3. Logout\n";
        std::cout << "Select Option: ";
        int choice;
        std::cin >> choice;
        if (choice == 3) break;
        if (choice == 1) {
            auto courses = courseService.listStudentCourses(user.userId);
            std::cout << "\n--- Enrolled Courses ---\n";
            if (courses.empty()) {
                std::cout << "You are not enrolled in any courses yet.\n";
            } else {
                for (const auto& c : courses) {
                    std::cout << "[" << c.courseCode << "] " << c.courseName << " (ID: " << c.courseId << ")\n";
                }
            }
        } else if (choice == 2) {
            auto assignments = assignService.listStudentAssignments(user.userId);
            std::cout << "\n--- Your Course Assignments ---\n";
            if (assignments.empty()) {
                std::cout << "No pending assignments found in your enrolled courses.\n";
                continue;
            }
            for (const auto& a : assignments) {
                std::cout << "Assignment ID [" << a.assignmentId << "] (Course ID: " << a.courseId << ") Title: " << a.title 
                          << " (Due: " << a.dueDate << ", Max: " << a.maxScore << ")\n";
            }
            std::cout << "Enter Assignment ID to submit for: ";
            int aid;
            std::cin >> aid;
            std::string filePath;
            std::cout << "Enter path to submission file: ";
            std::cin >> filePath;
            if (assignService.submitAssignment(aid, user.userId, filePath)) {
                std::cout << "SUCCESS: Solution file submitted successfully! SHA-256 fingerprint generated.\n";
            } else {
                std::cout << "ERROR: Submission failed. Verify that you are enrolled in the course for this assignment.\n";
            }
        }
    }
}

void ConsoleUI::handleLecturerMenu(const User& user) {
    CourseService courseService(db);
    AssignmentService assignService(db);

    while (true) {
        std::cout << "\n--- LECTURER MENU (User: " << user.username << ") ---\n";
        std::cout << "1. View My Assigned Courses & Enrolled Students\n";
        std::cout << "2. Create Course Assignment\n";
        std::cout << "3. Grade Student Submission (with Mandatory Audit Reason)\n";
        std::cout << "4. Grant Class-Wide Deadline Extension (CR1)\n";
        std::cout << "5. Logout\n";
        std::cout << "Select Option: ";
        int choice;
        std::cin >> choice;
        if (choice == 5) break;
        if (choice == 1) {
            auto courses = courseService.listLecturerCourses(user.userId);
            if (courses.empty()) {
                std::cout << "No courses assigned to you.\n";
            } else {
                for (const auto& c : courses) {
                    std::cout << "\nCourse: [" << c.courseCode << "] " << c.courseName << " (ID: " << c.courseId << ")\n";
                    auto students = courseService.listEnrolledStudents(c.courseId);
                    std::cout << "  Enrolled Students (" << students.size() << "):\n";
                    for (const auto& s : students) {
                        std::cout << "   - " << s.username << " (ID: " << s.userId << ")\n";
                    }
                }
            }
        } else if (choice == 2) {
            auto courses = courseService.listLecturerCourses(user.userId);
            if (courses.empty()) {
                std::cout << "ERROR: You cannot create assignments because you have no assigned courses.\n";
                continue;
            }
            std::cout << "Your Courses:\n";
            for (const auto& c : courses) {
                std::cout << "ID [" << c.courseId << "] " << c.courseCode << " - " << c.courseName << "\n";
            }
            std::cout << "Enter Course ID: ";
            int cid;
            std::cin >> cid;
            std::string title, dueDate;
            double maxScore;
            std::cout << "Enter Assignment Title: ";
            std::cin.ignore();
            std::getline(std::cin, title);
            std::cout << "Enter Due Date (YYYY-MM-DD HH:MM:SS): ";
            std::getline(std::cin, dueDate);
            std::cout << "Enter Max Score: ";
            std::cin >> maxScore;
            if (assignService.createAssignment(cid, title, dueDate, maxScore)) {
                std::cout << "SUCCESS: Assignment created under Course ID " << cid << ".\n";
            } else {
                std::cout << "ERROR: Failed to create assignment.\n";
            }
        } else if (choice == 3) {
            std::cout << "Enter Target Submission ID: ";
            int subId;
            double mark;
            std::string reason;
            std::cin >> subId;
            std::cout << "Enter Raw Mark: ";
            std::cin >> mark;
            std::cout << "Enter Mandatory Justification Reason: ";
            std::cin.ignore();
            std::getline(std::cin, reason);
            if (assignService.gradeSubmission(subId, mark, user.userId, reason)) {
                std::cout << "SUCCESS: Mark recorded in PROVISIONAL state and written to audit log.\n";
            }
        } else if (choice == 4) {
            std::cout << "Enter Assignment ID: ";
            int aid;
            std::string newDate;
            std::cin >> aid;
            std::cout << "Enter Extended Class-Wide Due Date: ";
            std::cin.ignore();
            std::getline(std::cin, newDate);
            if (assignService.extendClassDeadline(aid, newDate, user.userId)) {
                std::cout << "SUCCESS: Class deadline updated (CR1).\n";
            }
        }
    }
}

void ConsoleUI::handleHodMenu(const User& user) {
    while (true) {
        std::cout << "\n--- HEAD OF DEPARTMENT (HoD) MENU (User: " << user.username << ") ---\n";
        std::cout << "1. Review & Approve Departmental Grades (Release to Students)\n";
        std::cout << "2. Logout\n";
        std::cout << "Select Option: ";
        int choice;
        std::cin >> choice;
        if (choice == 2) break;
        if (choice == 1) {
            std::cout << "Enter Submission ID to Approve & Release: ";
            int subId;
            std::cin >> subId;
            AssignmentService assignService(db);
            if (assignService.moderateSubmission(subId, true, user.userId)) {
                std::cout << "SUCCESS: Grade moderated to MODERATED status.\n";
            }
        }
    }
}

void ConsoleUI::handleAdminMenu(const User& user) {
    AuthService auth(db);
    CourseService courseService(db);

    while (true) {
        std::cout << "\n--- SYSTEM ADMINISTRATOR MENU (User: " << user.username << ") ---\n";
        std::cout << "1. Create New System User Account\n";
        std::cout << "2. Create New Course & Assign Lecturer\n";
        std::cout << "3. Enroll Student into Course\n";
        std::cout << "4. List All Courses & Enrolled Students\n";
        std::cout << "5. Perform SQLite Maintenance & Database Snapshot\n";
        std::cout << "6. Logout\n";
        std::cout << "Select Option: ";
        int choice;
        std::cin >> choice;
        if (choice == 6) break;
        if (choice == 1) {
            std::string uname, pwd, rStr;
            std::cout << "Enter New Username: ";
            std::cin >> uname;
            std::cout << "Enter Password: ";
            std::cin >> pwd;
            std::cout << "Assign Role (STUDENT, LECTURER, HOD, ADMIN): ";
            std::cin >> rStr;
            auth.registerUser(uname, pwd, stringToRole(rStr));
            std::cout << "SUCCESS: User account registered successfully.\n";
        } else if (choice == 2) {
            std::string code, name;
            std::cout << "Enter Course Code (e.g., CS4433): ";
            std::cin >> code;
            std::cout << "Enter Course Name: ";
            std::cin.ignore();
            std::getline(std::cin, name);

            auto lecturers = auth.listUsersByRole(UserRole::LECTURER);
            if (lecturers.empty()) {
                std::cout << "ERROR: No lecturers found in the system. Create a Lecturer account first.\n";
                continue;
            }
            std::cout << "Available Lecturers:\n";
            for (const auto& l : lecturers) {
                std::cout << "  ID [" << l.userId << "] " << l.username << "\n";
            }
            std::cout << "Enter Lecturer User ID to assign: ";
            int lid;
            std::cin >> lid;
            if (courseService.createCourse(code, name, lid)) {
                std::cout << "SUCCESS: Course " << code << " created and assigned to lecturer.\n";
            } else {
                std::cout << "ERROR: Failed to create course. Ensure code is unique.\n";
            }
        } else if (choice == 3) {
            auto courses = courseService.listAllCourses();
            auto students = auth.listUsersByRole(UserRole::STUDENT);
            if (courses.empty() || students.empty()) {
                std::cout << "ERROR: Requires at least one course and one student to exist.\n";
                continue;
            }
            std::cout << "\nAvailable Courses:\n";
            for (const auto& c : courses) {
                std::cout << "  ID [" << c.courseId << "] " << c.courseCode << " - " << c.courseName << "\n";
            }
            std::cout << "Enter Course ID: ";
            int cid;
            std::cin >> cid;

            std::cout << "\nAvailable Students:\n";
            for (const auto& s : students) {
                std::cout << "  ID [" << s.userId << "] " << s.username << "\n";
            }
            std::cout << "Enter Student User ID: ";
            int sid;
            std::cin >> sid;

            if (courseService.enrollStudentInCourse(cid, sid)) {
                std::cout << "SUCCESS: Student enrolled into course successfully.\n";
            } else {
                std::cout << "ERROR: Student is already enrolled in this course or invalid IDs provided.\n";
            }
        } else if (choice == 4) {
            auto courses = courseService.listAllCourses();
            std::cout << "\n=== All Courses & Roster Overview ===\n";
            for (const auto& c : courses) {
                std::cout << "\nCourse: [" << c.courseCode << "] " << c.courseName << " (ID: " << c.courseId << ")\n";
                auto enrolled = courseService.listEnrolledStudents(c.courseId);
                std::cout << "  Enrolled Roster (" << enrolled.size() << " students):\n";
                for (const auto& s : enrolled) {
                    std::cout << "   - Student ID: " << s.userId << " | Username: " << s.username << "\n";
                }
            }
        } else if (choice == 5) {
            db.execute("VACUUM;");
            std::cout << "SUCCESS: SQLite VACUUM optimization executed.\n";
        }
    }
}
