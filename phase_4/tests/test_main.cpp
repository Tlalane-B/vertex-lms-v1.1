#include "Database.h"
#include "Repository.h"
#include "Service.h"
#include <iostream>
#include <cassert>

void runTests() {
    std::cout << "[TEST] Initializing Phase 4 Test Suite in in-memory / test DB...\n";
    {
        Database testDb("test_vertex_lms.db");
        
        // Test UserRepository & AuthService
        AuthService auth(testDb);
        bool reg = auth.registerUser("test_student", "SecurePass1!", UserRole::STUDENT);
        assert(reg && "User registration failed");

        auto loginOpt = auth.login("test_student", "SecurePass1!");
        assert(loginOpt.has_value() && "Authentication verification failed");
        assert(loginOpt->role == UserRole::STUDENT && "Role binding incorrect");

        // Test Course Creation & Enrollment
        CourseService courseService(testDb);
        bool createCourse = auth.registerUser("test_lect", "Lecturer1!", UserRole::LECTURER);
        assert(createCourse);
        auto lectOpt = auth.login("test_lect", "Lecturer1!");
        
        bool cRes = courseService.createCourse("CS9999", "Advanced Testing", lectOpt->userId);
        assert(cRes && "Course creation failed");

        auto courses = courseService.listAllCourses();
        assert(!courses.empty() && "Course list empty");

        int courseId = courses[0].courseId;
        bool enroll = courseService.enrollStudentInCourse(courseId, loginOpt->userId);
        assert(enroll && "Student enrollment failed");

        // Test Assignment & Submission
        AssignmentService assignService(testDb);
        bool assignRes = assignService.createAssignment(courseId, "Test Lab 1", "2026-12-31 23:59:59", 50.0);
        assert(assignRes && "Assignment creation failed");

        auto studentAssigns = assignService.listStudentAssignments(loginOpt->userId);
        assert(!studentAssigns.empty() && "Student assignment retrieval failed");

        bool subRes = assignService.submitAssignment(studentAssigns[0].assignmentId, loginOpt->userId, "solution.cpp");
        assert(subRes && "Submission creation failed");

        std::cout << "[SUCCESS] ALL PHASE 4 AUTOMATED UNIT & INTEGRATION TESTS PASSED!\n";
    }
    std::remove("test_vertex_lms.db");
}

int main() {
    runTests();
    return 0;
}
