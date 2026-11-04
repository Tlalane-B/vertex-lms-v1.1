#include "Analytics.h"
#include "Repository.h"
#include <iostream>
#include <fstream>

void AnalyticsModule::generateSystemReport() const {
    UserRepository userRepo(db);
    CourseRepository courseRepo(db);
    AssignmentRepository assignRepo(db);
    SubmissionRepository subRepo(db);

    auto students = userRepo.getUsersByRole(UserRole::STUDENT);
    auto lecturers = userRepo.getUsersByRole(UserRole::LECTURER);
    auto hods = userRepo.getUsersByRole(UserRole::HOD);
    auto admins = userRepo.getUsersByRole(UserRole::ADMIN);
    auto courses = courseRepo.getAllCourses();
    auto assignments = assignRepo.getAllAssignments();

    std::cout << "\n===================================================\n";
    std::cout << "      VERTEX LMS v1.0 - FINAL PRODUCTION REPORT     \n";
    std::cout << "===================================================\n";
    std::cout << " [DEMOGRAPHICS & ROSTER]                         \n";
    std::cout << "  - Total Students Registered : " << students.size() << "\n";
    std::cout << "  - Total Lecturers           : " << lecturers.size() << "\n";
    std::cout << "  - Total HoDs                : " << hods.size() << "\n";
    std::cout << "  - Total Administrators      : " << admins.size() << "\n";
    std::cout << " [ACADEMIC STRUCTURE]                            \n";
    std::cout << "  - Total Active Courses      : " << courses.size() << "\n";
    std::cout << "  - Total Active Assignments  : " << assignments.size() << "\n";
    std::cout << " [SUBMISSION & MODERATION METRICS]               \n";
    std::cout << "  - Total Submissions Uploaded: " << subRepo.getTotalSubmissionCount() << "\n";
    std::cout << "  - Total Moderated Grades    : " << subRepo.getModeratedSubmissionCount() << "\n";
    std::cout << "===================================================\n";
}

bool AnalyticsModule::exportAuditTrailCSV(const std::string& outputPath) const {
    std::ofstream file(outputPath);
    if (!file.is_open()) return false;

    file << "LogID,SubmissionID,OldMark,NewMark,ModifiedBy,Timestamp,Reason\n";
    
    std::string sql = "SELECT log_id, submission_id, old_mark, new_mark, modified_by, timestamp, reason FROM audit_logs;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        file << sqlite3_column_int(stmt, 0) << ","
             << sqlite3_column_int(stmt, 1) << ",";
        if (sqlite3_column_type(stmt, 2) == SQLITE_NULL) {
            file << "N/A,";
        } else {
            file << sqlite3_column_double(stmt, 2) << ",";
        }
        file << sqlite3_column_double(stmt, 3) << ","
             << sqlite3_column_int(stmt, 4) << ",""
             << sqlite3_column_text(stmt, 5) << "",""
             << sqlite3_column_text(stmt, 6) << ""\n";
    }
    sqlite3_finalize(stmt);
    file.close();
    return true;
}
