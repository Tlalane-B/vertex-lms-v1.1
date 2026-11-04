#ifndef SERVICE_H
#define SERVICE_H

#include "Repository.h"
#include <string>
#include <memory>

/**
 * @class AuthService
 * @brief Handles user authentication, password hashing, salt generation, and user management.
 */
class AuthService {
private:
    UserRepository userRepo;
public:
    explicit AuthService(Database& db) : userRepo(db) {}

    bool registerUser(const std::string& username, const std::string& password, UserRole role);
    std::optional<User> login(const std::string& username, const std::string& password);
    std::string hashPassword(const std::string& password, const std::string& salt);
    std::string generateSalt();
    std::vector<User> listUsersByRole(UserRole role);
};

/**
 * @class CourseService
 * @brief Encapsulates business logic for course creation and student enrollments.
 */
class CourseService {
private:
    CourseRepository courseRepo;
    UserRepository userRepo;
public:
    explicit CourseService(Database& db) : courseRepo(db), userRepo(db) {}

    bool createCourse(const std::string& courseCode, const std::string& courseName, int lecturerId);
    bool enrollStudentInCourse(int courseId, int studentId);
    std::vector<Course> listAllCourses();
    std::vector<Course> listLecturerCourses(int lecturerId);
    std::vector<Course> listStudentCourses(int studentId);
    std::vector<User> listEnrolledStudents(int courseId);
};

/**
 * @class AssignmentService
 * @brief Encapsulates business logic rules for course assignments, submissions, moderation, and penalties.
 */
class AssignmentService {
private:
    AssignmentRepository assignRepo;
    SubmissionRepository subRepo;
    CourseRepository courseRepo;
public:
    explicit AssignmentService(Database& db) : assignRepo(db), subRepo(db), courseRepo(db) {}

    bool createAssignment(int courseId, const std::string& title, const std::string& dueDate, double maxScore);
    std::vector<Assignment> listAssignmentsByCourse(int courseId);
    std::vector<Assignment> listStudentAssignments(int studentId);
    bool submitAssignment(int assignmentId, int studentId, const std::string& filePath);
    bool gradeSubmission(int submissionId, double mark, int lecturerId, const std::string& reason);
    bool moderateSubmission(int submissionId, bool approve, int hodId);
    bool extendClassDeadline(int assignmentId, const std::string& newDueDate, int lecturerId);
    double calculateLatePenalty(const std::string& dueDate, const std::string& subDate, double rawMark);
};

#endif // SERVICE_H
