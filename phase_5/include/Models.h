#ifndef MODELS_H
#define MODELS_H

#include <string>

enum class UserRole {
    STUDENT,
    LECTURER,
    HOD,
    ADMIN
};

inline std::string roleToString(UserRole role) {
    switch(role) {
        case UserRole::STUDENT: return "STUDENT";
        case UserRole::LECTURER: return "LECTURER";
        case UserRole::HOD: return "HOD";
        case UserRole::ADMIN: return "ADMIN";
    }
    return "STUDENT";
}

inline UserRole stringToRole(const std::string& str) {
    if(str == "LECTURER") return UserRole::LECTURER;
    if(str == "HOD") return UserRole::HOD;
    if(str == "ADMIN") return UserRole::ADMIN;
    return UserRole::STUDENT;
}

struct User {
    int userId;
    std::string username;
    std::string passwordHash;
    std::string salt;
    UserRole role;
};

struct Course {
    int courseId;
    std::string courseCode;
    std::string courseName;
    int lecturerId;
};

struct CourseEnrollment {
    int enrollmentId;
    int courseId;
    int studentId;
    std::string enrolledAt;
};

struct Assignment {
    int assignmentId;
    int courseId;
    std::string title;
    std::string dueDate;
    double maxScore;
};

enum class SubmissionStatus {
    PROVISIONAL,
    MODERATED
};

inline std::string statusToString(SubmissionStatus status) {
    return status == SubmissionStatus::MODERATED ? "MODERATED" : "PROVISIONAL";
}

inline SubmissionStatus stringToStatus(const std::string& str) {
    return str == "MODERATED" ? SubmissionStatus::MODERATED : SubmissionStatus::PROVISIONAL;
}

struct Submission {
    int submissionId;
    int assignmentId;
    int studentId;
    std::string filePath;
    std::string fileHash;
    std::string timestamp;
    SubmissionStatus status;
    double rawMark;
    bool hasMark;
};

struct AuditLog {
    int logId;
    int submissionId;
    double oldMark;
    double newMark;
    int modifiedBy;
    std::string timestamp;
    std::string reason;
};

#endif // MODELS_H
