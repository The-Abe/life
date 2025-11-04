cmake_minimum_required(VERSION 3.10)

set(PROJECT life)
project(${PROJECT})

set(CMAKE_CXX_STANDARD 20)
set(CXX /usr/bin/clang++)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/include/")

add_executable(app ${SOURCES})

target_include_directories(app PRIVATE ${PROJECT_INCLUDE})
target_link_libraries(app PRIVATE raylib)
