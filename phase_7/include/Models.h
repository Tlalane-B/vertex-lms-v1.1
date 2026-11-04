#ifndef MODELS_H
#define MODELS_H

#include <string>

/**
 * @enum UserRole
 * @brief Represents System Role-Based Access Control (RBAC) levels.
 */
enum class UserRole {
    STUDENT,   ///< Can view enrolled courses, submit assignments, view released marks
    LECTURER,  ///< Can manage assigned courses, create assignments, grade work, grant extensions (CR1)
    HOD,       ///< Head of Department: moderates marks, releases grades, views audit logs
    ADMIN      ///< System Administrator: user account management, course management, enrollment & DB backup
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

/**
 * @struct User
 * @brief Model representing a system user entity in the database.
 */
struct User {
    int userId;                  ///< Primary key
    std::string username;        ///< Unique login handle
    std::string passwordHash;    ///< Cryptographic salted password hash
    std::string salt;            ///< 16-byte random salt string
    UserRole role;               ///< Assigned RBAC role
};

/**
 * @struct Course
 * @brief Model representing an academic course entity.
 */
struct Course {
    int courseId;                ///< Primary key
    std::string courseCode;      ///< Unique course code (e.g., CS4433)
    std::string courseName;      ///< Full descriptive name of the course
    int lecturerId;              ///< Foreign key -> users.user_id (Assigned Lecturer)
};

/**
 * @struct CourseEnrollment
 * @brief Junction table model representing student enrollment in a course.
 */
struct CourseEnrollment {
    int enrollmentId;            ///< Primary key
    int courseId;                ///< Foreign key -> courses.course_id
    int studentId;               ///< Foreign key -> users.user_id
    std::string enrolledAt;      ///< ISO timestamp of enrollment
};

/**
 * @struct Assignment
 * @brief Model representing an assignment belonging to a specific course.
 */
struct Assignment {
    int assignmentId;            ///< Primary key
    int courseId;                ///< Foreign key -> courses.course_id
    std::string title;           ///< Assignment title
    std::string dueDate;         ///< ISO format deadline timestamp (YYYY-MM-DD HH:MM:SS)
    double maxScore;             ///< Maximum achievable grade points
};

/**
 * @enum SubmissionStatus
 * @brief Lifecycle workflow state of a student submission.
 */
enum class SubmissionStatus {
    PROVISIONAL,  ///< Grade entered by lecturer, hidden from student until HoD approval
    MODERATED    ///< Grade approved by HoD, visible to student
};

inline std::string statusToString(SubmissionStatus status) {
    return status == SubmissionStatus::MODERATED ? "MODERATED" : "PROVISIONAL";
}

inline SubmissionStatus stringToStatus(const std::string& str) {
    return str == "MODERATED" ? SubmissionStatus::MODERATED : SubmissionStatus::PROVISIONAL;
}

/**
 * @struct Submission
 * @brief Model representing an assignment submission by a student.
 */
struct Submission {
    int submissionId;            ///< Primary key
    int assignmentId;            ///< Foreign key -> assignments.assignment_id
    int studentId;               ///< Foreign key -> users.user_id
    std::string filePath;        ///< Local storage path of submitted file
    std::string fileHash;        ///< Cryptographic SHA-256 hash for integrity validation
    std::string timestamp;       ///< Upload timestamp
    SubmissionStatus status;     ///< Current workflow state (PROVISIONAL/MODERATED)
    double rawMark;              ///< Numerical mark assigned by lecturer
    bool hasMark;                ///< Flag indicating if grade has been populated
};

/**
 * @struct AuditLog
 * @brief Model representing an immutable record change in the audit trail.
 */
struct AuditLog {
    int logId;                   ///< Primary key
    int submissionId;            ///< Foreign key -> submissions.submission_id
    double oldMark;              ///< Previous numerical mark (-1.0 if initial entry)
    double newMark;              ///< Updated numerical mark
    int modifiedBy;              ///< Foreign key -> users.user_id (Lecturer/HoD)
    std::string timestamp;       ///< Exact time of grade modification
    std::string reason;          ///< Mandatory textual justification reason for audit
};

#endif // MODELS_H
