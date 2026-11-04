#include "Repository.h"
#include <iostream>

bool UserRepository::createUser(const User& user) {
    std::string sql = "INSERT INTO users (username, password_hash, salt, role) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, user.username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user.passwordHash.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, user.salt.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, roleToString(user.role).c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

std::optional<User> UserRepository::findByUsername(const std::string& username) {
    std::string sql = "SELECT user_id, username, password_hash, salt, role FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return std::nullopt;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    std::optional<User> user = std::nullopt;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        User u;
        u.userId = sqlite3_column_int(stmt, 0);
        u.username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        u.passwordHash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        u.salt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        u.role = stringToRole(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        user = u;
    }
    sqlite3_finalize(stmt);
    return user;
}

std::optional<User> UserRepository::findById(int userId) {
    std::string sql = "SELECT user_id, username, password_hash, salt, role FROM users WHERE user_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return std::nullopt;

    sqlite3_bind_int(stmt, 1, userId);
    std::optional<User> user = std::nullopt;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        User u;
        u.userId = sqlite3_column_int(stmt, 0);
        u.username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        u.passwordHash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        u.salt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        u.role = stringToRole(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        user = u;
    }
    sqlite3_finalize(stmt);
    return user;
}

std::vector<User> UserRepository::getUsersByRole(UserRole role) {
    std::vector<User> list;
    std::string sql = "SELECT user_id, username, password_hash, salt, role FROM users WHERE role = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return list;

    sqlite3_bind_text(stmt, 1, roleToString(role).c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        User u;
        u.userId = sqlite3_column_int(stmt, 0);
        u.username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        u.passwordHash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        u.salt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        u.role = stringToRole(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        list.push_back(u);
    }
    sqlite3_finalize(stmt);
    return list;
}

bool CourseRepository::createCourse(Course& course) {
    std::string sql = "INSERT INTO courses (course_code, course_name, lecturer_id) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, course.courseCode.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, course.courseName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, course.lecturerId);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    if (success) {
        course.courseId = static_cast<int>(sqlite3_last_insert_rowid(db.getConnection()));
    }
    sqlite3_finalize(stmt);
    return success;
}

std::optional<Course> CourseRepository::findById(int courseId) {
    std::string sql = "SELECT course_id, course_code, course_name, lecturer_id FROM courses WHERE course_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return std::nullopt;

    sqlite3_bind_int(stmt, 1, courseId);
    std::optional<Course> course = std::nullopt;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        Course c;
        c.courseId = sqlite3_column_int(stmt, 0);
        c.courseCode = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        c.courseName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        c.lecturerId = sqlite3_column_int(stmt, 3);
        course = c;
    }
    sqlite3_finalize(stmt);
    return course;
}

std::optional<Course> CourseRepository::findByCode(const std::string& courseCode) {
    std::string sql = "SELECT course_id, course_code, course_name, lecturer_id FROM courses WHERE course_code = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return std::nullopt;

    sqlite3_bind_text(stmt, 1, courseCode.c_str(), -1, SQLITE_STATIC);
    std::optional<Course> course = std::nullopt;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        Course c;
        c.courseId = sqlite3_column_int(stmt, 0);
        c.courseCode = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        c.courseName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        c.lecturerId = sqlite3_column_int(stmt, 3);
        course = c;
    }
    sqlite3_finalize(stmt);
    return course;
}

std::vector<Course> CourseRepository::getAllCourses() {
    std::vector<Course> list;
    std::string sql = "SELECT course_id, course_code, course_name, lecturer_id FROM courses;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return list;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Course c;
        c.courseId = sqlite3_column_int(stmt, 0);
        c.courseCode = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        c.courseName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        c.lecturerId = sqlite3_column_int(stmt, 3);
        list.push_back(c);
    }
    sqlite3_finalize(stmt);
    return list;
}

std::vector<Course> CourseRepository::getCoursesByLecturer(int lecturerId) {
    std::vector<Course> list;
    std::string sql = "SELECT course_id, course_code, course_name, lecturer_id FROM courses WHERE lecturer_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return list;

    sqlite3_bind_int(stmt, 1, lecturerId);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Course c;
        c.courseId = sqlite3_column_int(stmt, 0);
        c.courseCode = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        c.courseName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        c.lecturerId = sqlite3_column_int(stmt, 3);
        list.push_back(c);
    }
    sqlite3_finalize(stmt);
    return list;
}

std::vector<Course> CourseRepository::getCoursesByStudent(int studentId) {
    std::vector<Course> list;
    std::string sql = "SELECT c.course_id, c.course_code, c.course_name, c.lecturer_id "
                      "FROM courses c JOIN course_enrollments e ON c.course_id = e.course_id "
                      "WHERE e.student_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return list;

    sqlite3_bind_int(stmt, 1, studentId);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Course c;
        c.courseId = sqlite3_column_int(stmt, 0);
        c.courseCode = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        c.courseName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        c.lecturerId = sqlite3_column_int(stmt, 3);
        list.push_back(c);
    }
    sqlite3_finalize(stmt);
    return list;
}

bool CourseRepository::enrollStudent(int courseId, int studentId, const std::string& timestamp) {
    std::string sql = "INSERT INTO course_enrollments (course_id, student_id, enrolled_at) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, courseId);
    sqlite3_bind_int(stmt, 2, studentId);
    sqlite3_bind_text(stmt, 3, timestamp.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool CourseRepository::isStudentEnrolled(int courseId, int studentId) {
    std::string sql = "SELECT enrollment_id FROM course_enrollments WHERE course_id = ? AND student_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, courseId);
    sqlite3_bind_int(stmt, 2, studentId);
    bool enrolled = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return enrolled;
}

std::vector<User> CourseRepository::getEnrolledStudents(int courseId) {
    std::vector<User> list;
    std::string sql = "SELECT u.user_id, u.username, u.password_hash, u.salt, u.role "
                      "FROM users u JOIN course_enrollments e ON u.user_id = e.student_id "
                      "WHERE e.course_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return list;

    sqlite3_bind_int(stmt, 1, courseId);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        User u;
        u.userId = sqlite3_column_int(stmt, 0);
        u.username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        u.passwordHash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        u.salt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        u.role = stringToRole(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        list.push_back(u);
    }
    sqlite3_finalize(stmt);
    return list;
}

bool AssignmentRepository::createAssignment(Assignment& assignment) {
    std::string sql = "INSERT INTO assignments (course_id, title, due_date, max_score) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, assignment.courseId);
    sqlite3_bind_text(stmt, 2, assignment.title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, assignment.dueDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, assignment.maxScore);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    if (success) {
        assignment.assignmentId = static_cast<int>(sqlite3_last_insert_rowid(db.getConnection()));
    }
    sqlite3_finalize(stmt);
    return success;
}

std::vector<Assignment> AssignmentRepository::getAllAssignments() {
    std::vector<Assignment> list;
    std::string sql = "SELECT assignment_id, course_id, title, due_date, max_score FROM assignments;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return list;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Assignment a;
        a.assignmentId = sqlite3_column_int(stmt, 0);
        a.courseId = sqlite3_column_int(stmt, 1);
        a.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        a.dueDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        a.maxScore = sqlite3_column_double(stmt, 4);
        list.push_back(a);
    }
    sqlite3_finalize(stmt);
    return list;
}

std::vector<Assignment> AssignmentRepository::getAssignmentsByCourse(int courseId) {
    std::vector<Assignment> list;
    std::string sql = "SELECT assignment_id, course_id, title, due_date, max_score FROM assignments WHERE course_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return list;

    sqlite3_bind_int(stmt, 1, courseId);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Assignment a;
        a.assignmentId = sqlite3_column_int(stmt, 0);
        a.courseId = sqlite3_column_int(stmt, 1);
        a.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        a.dueDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        a.maxScore = sqlite3_column_double(stmt, 4);
        list.push_back(a);
    }
    sqlite3_finalize(stmt);
    return list;
}

std::optional<Assignment> AssignmentRepository::findById(int assignmentId) {
    std::string sql = "SELECT assignment_id, course_id, title, due_date, max_score FROM assignments WHERE assignment_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return std::nullopt;

    sqlite3_bind_int(stmt, 1, assignmentId);
    std::optional<Assignment> assign = std::nullopt;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        Assignment a;
        a.assignmentId = sqlite3_column_int(stmt, 0);
        a.courseId = sqlite3_column_int(stmt, 1);
        a.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        a.dueDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        a.maxScore = sqlite3_column_double(stmt, 4);
        assign = a;
    }
    sqlite3_finalize(stmt);
    return assign;
}

bool AssignmentRepository::updateAssignmentDueDate(int assignmentId, const std::string& newDueDate) {
    std::string sql = "UPDATE assignments SET due_date = ? WHERE assignment_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, newDueDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, assignmentId);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool SubmissionRepository::createSubmission(Submission& sub) {
    std::string sql = "INSERT INTO submissions (assignment_id, student_id, file_path, file_hash, timestamp, status) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, sub.assignmentId);
    sqlite3_bind_int(stmt, 2, sub.studentId);
    sqlite3_bind_text(stmt, 3, sub.filePath.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, sub.fileHash.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, sub.timestamp.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, statusToString(sub.status).c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    if (success) {
        sub.submissionId = static_cast<int>(sqlite3_last_insert_rowid(db.getConnection()));
    }
    sqlite3_finalize(stmt);
    return success;
}

std::vector<Submission> SubmissionRepository::getSubmissionsByAssignment(int assignmentId) {
    std::vector<Submission> list;
    std::string sql = "SELECT submission_id, assignment_id, student_id, file_path, file_hash, timestamp, status, raw_mark FROM submissions WHERE assignment_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return list;

    sqlite3_bind_int(stmt, 1, assignmentId);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Submission s;
        s.submissionId = sqlite3_column_int(stmt, 0);
        s.assignmentId = sqlite3_column_int(stmt, 1);
        s.studentId = sqlite3_column_int(stmt, 2);
        s.filePath = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        s.fileHash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        s.timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        s.status = stringToStatus(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
        if (sqlite3_column_type(stmt, 7) == SQLITE_NULL) {
            s.hasMark = false;
            s.rawMark = 0.0;
        } else {
            s.hasMark = true;
            s.rawMark = sqlite3_column_double(stmt, 7);
        }
        list.push_back(s);
    }
    sqlite3_finalize(stmt);
    return list;
}

bool SubmissionRepository::updateMarkAndStatus(int submissionId, double mark, const std::string& status) {
    std::string sql = "UPDATE submissions SET raw_mark = ?, status = ? WHERE submission_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_double(stmt, 1, mark);
    sqlite3_bind_text(stmt, 2, status.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, submissionId);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool SubmissionRepository::logAudit(const AuditLog& log) {
    std::string sql = "INSERT INTO audit_logs (submission_id, old_mark, new_mark, modified_by, timestamp, reason) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, log.submissionId);
    if (log.oldMark < 0) {
        sqlite3_bind_null(stmt, 2);
    } else {
        sqlite3_bind_double(stmt, 2, log.oldMark);
    }
    sqlite3_bind_double(stmt, 3, log.newMark);
    sqlite3_bind_int(stmt, 4, log.modifiedBy);
    sqlite3_bind_text(stmt, 5, log.timestamp.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, log.reason.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

std::vector<AuditLog> SubmissionRepository::getAuditLogsForSubmission(int submissionId) {
    std::vector<AuditLog> list;
    std::string sql = "SELECT log_id, submission_id, old_mark, new_mark, modified_by, timestamp, reason FROM audit_logs WHERE submission_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return list;

    sqlite3_bind_int(stmt, 1, submissionId);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        AuditLog l;
        l.logId = sqlite3_column_int(stmt, 0);
        l.submissionId = sqlite3_column_int(stmt, 1);
        if (sqlite3_column_type(stmt, 2) == SQLITE_NULL) {
            l.oldMark = -1.0;
        } else {
            l.oldMark = sqlite3_column_double(stmt, 2);
        }
        l.newMark = sqlite3_column_double(stmt, 3);
        l.modifiedBy = sqlite3_column_int(stmt, 4);
        l.timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        l.reason = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        list.push_back(l);
    }
    sqlite3_finalize(stmt);
    return list;
}

int SubmissionRepository::getTotalSubmissionCount() {
    std::string sql = "SELECT COUNT(*) FROM submissions;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return 0;
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count;
}

int SubmissionRepository::getModeratedSubmissionCount() {
    std::string sql = "SELECT COUNT(*) FROM submissions WHERE status = 'MODERATED';";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return 0;
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count;
}
