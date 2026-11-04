# Vertex LMS v1.0 - User Manual & Deployment Guide

## System Requirements
- **OS:** Windows 10/11 (64-bit) or Linux/macOS with CMake & C++17 compiler toolchain.
- **Compiler:** MSVC (Visual Studio 2022), GCC 9+, or Clang 10+.
- **Dependencies:** SQLite3 development libraries (`sqlite3.h`, `sqlite3.lib` / `-lsqlite3`).

---

## Installation & First-Time Setup

1. **Extract Archive:** Extract `vertex_lms_v1.0_phase5.zip` to your target directory.
2. **Automated Launch (Windows):**
   - Double-click `run.bat` in the root folder.
   - Choose Option **[1]** to build and run the main application.
   - Choose Option **[2]** to execute the automated verification test suite.
3. **Manual CMake Build:**
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build . --config Release
   ```

---

## Default Baseline Accounts
Upon initial startup, Vertex LMS automatically seeds the system with baseline demonstration accounts:

- **System Administrator:** `admin` / `Admin123!`
- **Lecturer:** `lecturer1` / `Lecturer1!`
- **Head of Department (HoD):** `hod1` / `Hod1!`
- **Student:** `student1` / `Student1!`

---

## Operational Guide by Role

### 1. Administrator Operations
- **Create Users:** Select option 1 to register new Students, Lecturers, HoDs, or Admins.
- **Course Setup:** Select option 2 to create course entities (e.g., `CS4433 Software Engineering`) and link them to an assigned Lecturer.
- **Enrollments:** Select option 3 to enroll registered Students into specific courses.
- **Analytics & Backups:** Use options 5–7 to view real-time system metrics, export audit logs to CSV, or take database backups.

### 2. Lecturer Operations
- **View Roster:** View courses assigned to your account and inspect enrolled student rosters.
- **Create Assignments:** Add new assignments with explicit due dates and max scores.
- **Grade Submissions:** Enter raw marks accompanied by mandatory textual justification reasons (logged to the audit trail).
- **Deadline Extensions:** Extend deadlines class-wide for a course assignment.

### 3. Student Operations
- **Course Roster:** View all active courses in which you are enrolled.
- **Assignment Submissions:** Select an assignment, provide the local file path to your solution, and receive a digital file hash receipt.

### 4. Head of Department (HoD) Operations
- **Grade Moderation:** Review provisional marks submitted by lecturers and approve them to transition the status to `MODERATED`, making them visible to students.
