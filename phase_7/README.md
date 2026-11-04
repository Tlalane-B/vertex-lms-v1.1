# Vertex LMS v1.0 (Release Package & Prototype)

## Overview
Vertex LMS is a C++17 standalone, offline-capable Learning Management System built for High School Assignments and Assesments.

This version (**v1.0**) incorporates explicit **Course Entities**, **Lecturer Course Assignments**, **Administrator Student Enrollments**, and a root **`run.bat` launcher script** for Windows environments.

## Quick Start Launcher (Windows)
Double-click `run.bat` in the root project folder to automatically build and launch the application.

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
