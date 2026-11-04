#include "Service.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <ctime>

// ==========================================
// AuthService Implementation
// ==========================================

std::string AuthService::generateSalt() {
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string salt = "";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);
    for (int i = 0; i < 16; ++i) {
        salt += alphanum[dis(gen)];
    }
    return salt;
}

std::string AuthService::hashPassword(const std::string& password, const std::string& salt) {
    std::hash<std::string> hasher;
    std::string combined = password + salt;
    size_t hashVal = hasher(combined);
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hashVal;
    return ss.str();
}

bool AuthService::registerUser(const std::string& username, const std::string& password, UserRole role) {
    std::string salt = generateSalt();
    std::string hash = hashPassword(password, salt);
    User u{0, username, hash, salt, role};
    return userRepo.createUser(u);
}

std::optional<User> AuthService::login(const std::string& username, const std::string& password) {
    auto userOpt = userRepo.findByUsername(username);
    if (!userOpt) return std::nullopt;
    
    std::string computedHash = hashPassword(password, userOpt->salt);
    if (computedHash == userOpt->passwordHash) {
        return userOpt;
    }
    return std::nullopt;
}

std::vector<User> AuthService::listUsersByRole(UserRole role) {
    return userRepo.getUsersByRole(role);
}

// ==========================================
// CourseService Implementation
// ==========================================

bool CourseService::createCourse(const std::string& courseCode, const std::string& courseName, int lecturerId) {
    Course c{0, courseCode, courseName, lecturerId};
    return courseRepo.createCourse(c);
}

bool CourseService::enrollStudentInCourse(int courseId, int studentId) {
    if (courseRepo.isStudentEnrolled(courseId, studentId)) return false;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    return courseRepo.enrollStudent(courseId, studentId, oss.str());
}

std::vector<Course> CourseService::listAllCourses() {
    return courseRepo.getAllCourses();
}

std::vector<Course> CourseService::listLecturerCourses(int lecturerId) {
    return courseRepo.getCoursesByLecturer(lecturerId);
}

std::vector<Course> CourseService::listStudentCourses(int studentId) {
    return courseRepo.getCoursesByStudent(studentId);
}

std::vector<User> CourseService::listEnrolledStudents(int courseId) {
    return courseRepo.getEnrolledStudents(courseId);
}

// ==========================================
// AssignmentService Implementation
// ==========================================

bool AssignmentService::createAssignment(int courseId, const std::string& title, const std::string& dueDate, double maxScore) {
    Assignment a{0, courseId, title, dueDate, maxScore};
    return assignRepo.createAssignment(a);
}

std::vector<Assignment> AssignmentService::listAssignmentsByCourse(int courseId) {
    return assignRepo.getAssignmentsByCourse(courseId);
}

std::vector<Assignment> AssignmentService::listStudentAssignments(int studentId) {
    std::vector<Assignment> result;
    auto courses = courseRepo.getCoursesByStudent(studentId);
    for (const auto& course : courses) {
        auto assignList = assignRepo.getAssignmentsByCourse(course.courseId);
        result.insert(result.end(), assignList.begin(), assignList.end());
    }
    return result;
}

bool AssignmentService::submitAssignment(int assignmentId, int studentId, const std::string& filePath) {
    auto assignOpt = assignRepo.findById(assignmentId);
    if (!assignOpt) return false;

    // Verify student is enrolled in the course associated with this assignment
    if (!courseRepo.isStudentEnrolled(assignOpt->courseId, studentId)) {
        return false;
    }

    std::string fileHash = "sha256_mock_hash_" + filePath;
    
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    Submission s{0, assignmentId, studentId, filePath, fileHash, oss.str(), SubmissionStatus::PROVISIONAL, 0.0, false};
    return subRepo.createSubmission(s);
}

bool AssignmentService::gradeSubmission(int submissionId, double mark, int lecturerId, const std::string& reason) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    AuditLog log{0, submissionId, -1.0, mark, lecturerId, oss.str(), reason};
    subRepo.logAudit(log);

    return subRepo.updateMarkAndStatus(submissionId, mark, "PROVISIONAL");
}

bool AssignmentService::moderateSubmission(int submissionId, bool approve, int hodId) {
    std::string newStatus = approve ? "MODERATED" : "PROVISIONAL";
    return subRepo.updateMarkAndStatus(submissionId, 0.0, newStatus);
}

bool AssignmentService::extendClassDeadline(int assignmentId, const std::string& newDueDate, int /*lecturerId*/) {
    return assignRepo.updateAssignmentDueDate(assignmentId, newDueDate);
}

double AssignmentService::calculateLatePenalty(const std::string& dueDate, const std::string& subDate, double rawMark) {
    if (subDate > dueDate) {
        return rawMark * 0.9;
    }
    return rawMark;
}
