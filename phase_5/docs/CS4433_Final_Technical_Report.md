# CS4433 Software Engineering Individual Assignment: Final Technical & Architecture Report

**Project Title:** Vertex LMS (Offline-Capable Learning Management System)  
**Version:** v1.0 (Production Release)  
**Author:** Tlalane Sechele 202201293  
**Institution:** National University of Lesotho (NUL), Department of Computer Science  
**Module:** CS4433 Software Engineering  

---

## 1. System Architecture & Component Design
Vertex LMS is built following a clean multi-tier Layered Architecture enforcing separation of concerns across Data Persistence, Business Domain Services, Analytics/Audit, and Presentation layers.

- **Presentation Layer (`ConsoleUI`):** Handles interactive CLI workflows, session context, role-based menu routing, and formatted input validation.
- **Service Layer (`AuthService`, `CourseService`, `AssignmentService`):** Implements business domain rules, SHA-256 equivalent salted password hashing, grade moderation logic, and class-wide deadline extension constraints (CR1).
- **Data Access Layer (`UserRepository`, `CourseRepository`, `AssignmentRepository`, `SubmissionRepository`):** Encapsulates prepared SQL statements to eliminate SQL injection risks and manages CRUD interactions.
- **Persistence Engine (`Database`):** Manages SQLite DB connections, Foreign Key enforcement, VACUUM maintenance, schema migrations, and backup snapshots.
- **Analytics & Maintenance Layer (`AnalyticsModule`):** Generates operational performance metrics and exports compliance audit trails to standard CSV formats.

---

## 2. Relational Database Schema (3NF Compliance)
The system operates on an embedded SQLite database (`vertex_lms.db`) structured in Third Normal Form (3NF):

1. **`users`**: Stores user authentication data (`user_id`, `username`, `password_hash`, `salt`, `role`).
2. **`courses`**: Stores active course offerings (`course_id`, `course_code`, `course_name`, `lecturer_id`).
3. **`course_enrollments`**: Junction table for student-course mapping (`enrollment_id`, `course_id`, `student_id`, `enrolled_at`).
4. **`assignments`**: Defines coursework requirements (`assignment_id`, `course_id`, `title`, `due_date`, `max_score`).
5. **`submissions`**: Tracks student deliverables, digital file integrity hashes, and provisional/moderated grade states (`submission_id`, `assignment_id`, `student_id`, `file_path`, `file_hash`, `timestamp`, `status`, `raw_mark`).
6. **`audit_logs`**: Immutable security audit trail recording all mark alterations and mandatory justifications (`log_id`, `submission_id`, `old_mark`, `new_mark`, `modified_by`, `timestamp`, `reason`).

---

## 3. Role-Based Access Control (RBAC) Matrix

| Feature / Action | Student | Lecturer | Head of Department (HoD) | Administrator |
|---|:---:|:---:|:---:|:---:|
| **Account Management** | — | — | — | Full (Create Users) |
| **Course Allocation** | — | — | — | Create & Assign Lecturers |
| **Student Enrollment** | View Enrolled | — | — | Assign Students to Courses |
| **Assignment Management** | View Assigned | Create & Extend (CR1) | View | View |
| **Solution Submission** | Submit File & Hash | — | — | — |
| **Provisional Grading** | — | Grade + Audit Reason | — | — |
| **Grade Moderation & Release** | — | — | Moderate & Release | — |
| **System Backup & Export** | — | — | — | Backup DB & Export CSV |

---

## 4. Quality Assurance & Test Verification Summary
Phase 5 final sign-off includes complete verification via the automated C++ test suite (`VertexLMSTests`):
- **User Authentication:** Salted hash generation and role verification.
- **Academic Hierarchy Enforcement:** Verification that un-enrolled students cannot submit work to restricted course assignments.
- **Audit Integrity:** Validation that every grade change automatically populates an immutable record in `audit_logs`.
- **Database Resilience:** Verification of schema auto-creation and backup snapshot functionality.
