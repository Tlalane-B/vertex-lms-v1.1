cmake_minimum_required(VERSION 3.14)
project(VertexLMS VERSION 1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(VertexLMS
    src/main.cpp
    src/Database.cpp
    src/Repository.cpp
    src/Service.cpp
    src/Analytics.cpp
    src/UI.cpp
)
target_link_libraries(VertexLMS PRIVATE SQLite::SQLite3)

add_executable(VertexLMSTests
    tests/test_main.cpp
    src/Database.cpp
    src/Repository.cpp
    src/Service.cpp
    src/Analytics.cpp
)
target_link_libraries(VertexLMSTests PRIVATE SQLite::SQLite3)

if(MSVC)
    target_compile_options(VertexLMS PRIVATE /W4 /wx)
    target_compile_options(VertexLMSTests PRIVATE /W4 /wx)
else()
    target_compile_options(VertexLMS PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(VertexLMSTests PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
