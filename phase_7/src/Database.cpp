#include "Database.h"
#include <iostream>

Database::Database(const std::string& path) : db(nullptr), dbPath(path) {
    open();
    initializeSchema();
}

Database::~Database() {
    close();
}

bool Database::open() {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    execute("PRAGMA foreign_keys = ON;");
    return true;
}

void Database::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

sqlite3* Database::getConnection() const {
    return db;
}

bool Database::execute(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL execution error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool Database::initializeSchema() {
    // 3NF Relational Database Schema with Courses and Student Enrollments
    std::string schema = 
        // 1. Users table
        "CREATE TABLE IF NOT EXISTS users ("
        "    user_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    username TEXT NOT NULL UNIQUE,"
        "    password_hash TEXT NOT NULL,"
        "    salt TEXT NOT NULL,"
        "    role TEXT NOT NULL CHECK(role IN ('STUDENT', 'LECTURER', 'HOD', 'ADMIN'))"
        ");"
        // 2. Courses table linked to an assigned Lecturer
        "CREATE TABLE IF NOT EXISTS courses ("
        "    course_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    course_code TEXT NOT NULL UNIQUE,"
        "    course_name TEXT NOT NULL,"
        "    lecturer_id INTEGER NOT NULL,"
        "    FOREIGN KEY (lecturer_id) REFERENCES users(user_id) ON DELETE CASCADE"
        ");"
        // 3. Course enrollments junction table
        "CREATE TABLE IF NOT EXISTS course_enrollments ("
        "    enrollment_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    course_id INTEGER NOT NULL,"
        "    student_id INTEGER NOT NULL,"
        "    enrolled_at TEXT NOT NULL,"
        "    UNIQUE(course_id, student_id),"
        "    FOREIGN KEY (course_id) REFERENCES courses(course_id) ON DELETE CASCADE,"
        "    FOREIGN KEY (student_id) REFERENCES users(user_id) ON DELETE CASCADE"
        ");"
        // 4. Assignments metadata table linked to a Course
        "CREATE TABLE IF NOT EXISTS assignments ("
        "    assignment_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    course_id INTEGER NOT NULL,"
        "    title TEXT NOT NULL,"
        "    due_date TEXT NOT NULL,"
        "    max_score REAL NOT NULL,"
        "    FOREIGN KEY (course_id) REFERENCES courses(course_id) ON DELETE CASCADE"
        ");"
        // 5. Submissions table tracking raw marks and moderation states
        "CREATE TABLE IF NOT EXISTS submissions ("
        "    submission_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    assignment_id INTEGER NOT NULL,"
        "    student_id INTEGER NOT NULL,"
        "    file_path TEXT NOT NULL,"
        "    file_hash TEXT NOT NULL,"
        "    timestamp TEXT NOT NULL,"
        "    status TEXT NOT NULL DEFAULT 'PROVISIONAL' CHECK(status IN ('PROVISIONAL', 'MODERATED')),"
        "    raw_mark REAL DEFAULT NULL,"
        "    FOREIGN KEY (assignment_id) REFERENCES assignments(assignment_id) ON DELETE CASCADE,"
        "    FOREIGN KEY (student_id) REFERENCES users(user_id) ON DELETE CASCADE"
        ");"
        // 6. Audit logs table ensuring mandatory audit trail for grade modifications
        "CREATE TABLE IF NOT EXISTS audit_logs ("
        "    log_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    submission_id INTEGER NOT NULL,"
        "    old_mark REAL,"
        "    new_mark REAL NOT NULL,"
        "    modified_by INTEGER NOT NULL,"
        "    timestamp TEXT NOT NULL,"
        "    reason TEXT NOT NULL,"
        "    FOREIGN KEY (submission_id) REFERENCES submissions(submission_id) ON DELETE CASCADE,"
        "    FOREIGN KEY (modified_by) REFERENCES users(user_id)"
        ");";
    return execute(schema);
}
