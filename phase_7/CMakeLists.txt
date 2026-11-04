cmake_minimum_required(VERSION 3.14)
project(VertexLMS VERSION 1.0 LANGUAGES CXX)

# Enforce C++17 standard compliance
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find and link the mandatory SQLite3 library
find_package(SQLite3 REQUIRED)

# Include project header directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Executable target linking all translation units
add_executable(VertexLMS
    src/main.cpp
    src/Database.cpp
    src/Repository.cpp
    src/Service.cpp
    src/UI.cpp
)

# Link against SQLite3 engine
target_link_libraries(VertexLMS PRIVATE SQLite::SQLite3)

# Enable compiler warnings for rigorous code quality assurance
if(MSVC)
    target_compile_options(VertexLMS PRIVATE /W4 /wx)
else()
    target_compile_options(VertexLMS PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
