#include "Database.h"
#include "Repository.h"
#include "Service.h"
#include <iostream>
#include <cassert>

void runTests() {
    std::cout << "[TEST] Executing Vertex LMS v1.1 Regression Suite (CR1 Check)...\n";
    {
        Database testDb("test_vertex_lms.db");
        
        AuthService auth(testDb);
        bool reg = auth.registerUser("test_student", "SecurePass1!", UserRole::STUDENT);
        assert(reg && "User registration failed");

        auto loginOpt = auth.login("test_student", "SecurePass1!");
        assert(loginOpt.has_value() && "Authentication verification failed");

        CourseService courseService(testDb);
        bool createLect = auth.registerUser("test_lect", "Lecturer1!", UserRole::LECTURER);
        assert(createLect);
        auto lectOpt = auth.login("test_lect", "Lecturer1!");
        
        bool cRes = courseService.createCourse("CS9999", "Advanced Testing", lectOpt->userId);
        assert(cRes && "Course creation failed");

        auto courses = courseService.listAllCourses();
        int courseId = courses[0].courseId;
        bool enroll = courseService.enrollStudentInCourse(courseId, loginOpt->userId);
        assert(enroll && "Student enrollment failed");

        AssignmentService assignService(testDb);
        bool assignRes = assignService.createAssignment(courseId, "Test Lab 1", "2026-12-31 23:59:59", 50.0);
        assert(assignRes && "Assignment creation failed");

        auto studentAssignsBefore = assignService.listStudentAssignments(loginOpt->userId);
        assert(!studentAssignsBefore.empty() && "Student assignment retrieval failed");
        
        // CR1 Regression Test: Extend deadline and verify student view sync
        bool extendRes = assignService.extendClassDeadline(studentAssignsBefore[0].assignmentId, "2027-01-15 23:59:59", lectOpt->userId);
        assert(extendRes && "CR1 Class-wide deadline extension failed");

        auto studentAssignsAfter = assignService.listStudentAssignments(loginOpt->userId);
        assert(studentAssignsAfter[0].dueDate == "2027-01-15 23:59:59" && "CR1 deadline extension not reflected on student assignment list");

        std::cout << "[SUCCESS] ALL VERTEX LMS v1.1 CR1 REGRESSION TESTS PASSED SUCCESSFULLY!\n";
    }
    std::remove("test_vertex_lms.db");
}

int main() {
    runTests();
    return 0;
}
