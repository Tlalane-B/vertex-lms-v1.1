#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "Database.h"
#include "Models.h"
#include <vector>
#include <optional>

class UserRepository {
private:
    Database& db;
public:
    explicit UserRepository(Database& database) : db(database) {}

    bool createUser(const User& user);
    std::optional<User> findByUsername(const std::string& username);
    std::optional<User> findById(int userId);
    std::vector<User> getUsersByRole(UserRole role);
};

class CourseRepository {
private:
    Database& db;
public:
    explicit CourseRepository(Database& database) : db(database) {}

    bool createCourse(Course& course);
    std::optional<Course> findById(int courseId);
    std::optional<Course> findByCode(const std::string& courseCode);
    std::vector<Course> getAllCourses();
    std::vector<Course> getCoursesByLecturer(int lecturerId);
    std::vector<Course> getCoursesByStudent(int studentId);

    bool enrollStudent(int courseId, int studentId, const std::string& timestamp);
    bool isStudentEnrolled(int courseId, int studentId);
    std::vector<User> getEnrolledStudents(int courseId);
};

class AssignmentRepository {
private:
    Database& db;
public:
    explicit AssignmentRepository(Database& database) : db(database) {}

    bool createAssignment(Assignment& assignment);
    std::vector<Assignment> getAllAssignments();
    std::vector<Assignment> getAssignmentsByCourse(int courseId);
    std::optional<Assignment> findById(int assignmentId);
    bool updateAssignmentDueDate(int assignmentId, const std::string& newDueDate);
};

class SubmissionRepository {
private:
    Database& db;
public:
    explicit SubmissionRepository(Database& database) : db(database) {}

    bool createSubmission(Submission& sub);
    std::vector<Submission> getSubmissionsByAssignment(int assignmentId);
    bool updateMarkAndStatus(int submissionId, double mark, const std::string& status);
    bool logAudit(const AuditLog& log);
    std::vector<AuditLog> getAuditLogsForSubmission(int submissionId);
    int getTotalSubmissionCount();
    int getModeratedSubmissionCount();
};

#endif // REPOSITORY_H
