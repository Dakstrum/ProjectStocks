cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(ProjectStocks)

find_package(SQLite3 REQUIRED)
include_directories(${SQLITE3_INCLUDE_DIRS})

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/src/*h)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    string(APPEND CMAKE_C_FLAGS " -lm")
    string(APPEND CMAKE_C_FLAGS_DEBUG " -lm")
    string(APPEND CMAKE_C_FLAGS_RELEASE " -lm")
endif()

string(APPEND CMAKE_C_FLAGS " -O3")
string(APPEND CMAKE_C_FLAGS_RELEASE " -O3")

add_executable(projectstocks ${sources} ${headers})
target_link_libraries(projectstocks -lallegro -lsqlite3)
