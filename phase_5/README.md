# Vertex LMS v1.0 - Phase 5 (Final Deployment & Release Sign-Off)

## Overview
Vertex LMS is a C++17 standalone, offline-capable Learning Management System built for High School Assignments and Assesments.

**Phase 5 Deliverables** mark final production deployment and academic project sign-off:
1. **Final Technical & Architecture Specification Report** (`docs/CS4433_Final_Technical_Report.md`).
2. **User Manual & Deployment Guide** (`docs/User_Manual_and_Deployment_Guide.md`).
3. **Automated Verification Suite** (`tests/test_main.cpp`).
4. **Interactive Production Launcher** (`run.bat`).

## Quick Start (Windows Launcher)
Double-click `run.bat` to launch the interactive menu to run the application or execute acceptance tests.

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
- **Admin:** `admin` / `Admin123!`
- **Lecturer:** `lecturer1` / `Lecturer1!`
- **HoD:** `hod1` / `Hod1!`
- **Student:** `student1` / `Student1!`
