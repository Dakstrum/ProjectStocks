cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(ProjectStocks)

#find_package(SQLite3 REQUIRED)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")

    # use, i.e. don't skip the full RPATH for the build tree
    SET(CMAKE_SKIP_BUILD_RPATH FALSE)

    # the RPATH to be used when installing
    SET(CMAKE_INSTALL_RPATH "lib")

    # when building, don't use the install RPATH already
    # (but later on when installing)
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

    # don't add the automatically determined parts of the RPATH
    # which point to directories outside the build tree to the install RPATH
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

endif()

include_directories(${SQLITE3_INCLUDE_DIRS})
include_directories(${JSON_C_PUBLIC_HEADERS})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.h)

string(APPEND CMAKE_C_FLAGS_DEBUG " -lm -O0")
string(APPEND CMAKE_C_FLAGS_RELEASE " -lm -O2")

add_executable(projectstocks ${sources} ${headers})

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")

    target_link_libraries(projectstocks -lallegro_main -lallegro -lallegro_audio -lallegro_acodec -lallegro_image -lallegro_video -lallegro_font -lallegro_ttf -lsqlite3 -ljson-c)

else()

    target_link_libraries(projectstocks -lallegro -lallegro_audio -lallegro_acodec -lallegro_image -lallegro_video -lallegro_font -lallegro_ttf -lsqlite3 -ljson-c)
    
endif()


