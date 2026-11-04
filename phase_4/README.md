# Vertex LMS v1.0 - Phase 4 (Production & QA Release)

## Overview
Vertex LMS is a C++17 standalone, offline-capable Learning Management System built for High School Assignments and Assesments.

**Phase 4 Deliverables** introduce:
1. **Automated Unit & Integration Test Suite** (`tests/test_main.cpp`) verifying persistence, authentication, course enrollment, and submission logic.
2. **System Analytics & Reporting Module** (`src/Analytics.cpp`) generating operational metrics and demographic rollups.
3. **Audit Trail CSV Export & Database Backup Utilities**.
4. **Interactive Launcher Script (`run.bat`)** supporting both application execution and automated test suite verification.

## Quick Start (Windows)
Double-click `run.bat` to launch the interactive menu where you can choose between running the application or executing the Phase 4 test suite.

## Build Instructions (Manual CMake)
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running the Application
```bash
./VertexLMS
```

## Default Accounts
- **Admin**: `admin` / `Admin123!`
- **Lecturer**: `lecturer1` / `Lecturer1!`
- **HoD**: `hod1` / `Hod1!`
- **Student**: `student1` / `Student1!`
