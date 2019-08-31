cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(ProjectStocks)

#find_package(SQLite3 REQUIRED)

include_directories(${SQLITE3_INCLUDE_DIRS})
include_directories(${JSON_C_PUBLIC_HEADERS})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.h)

string(APPEND CMAKE_C_FLAGS " -lm -O3")
string(APPEND CMAKE_C_FLAGS_DEBUG " -lm")
string(APPEND CMAKE_C_FLAGS_RELEASE " -lm -O3")

add_executable(projectstocks ${sources} ${headers})
target_link_libraries(projectstocks -lallegro -lallegro_audio -lallegro_acodec -lallegro_image -lallegro_video -lallegro_font -lsqlite3 -ljson-c)
