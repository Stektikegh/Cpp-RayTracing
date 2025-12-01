
cmake_minimum_required(VERSION 3.25)
project(rayyyy)

set(CMAKE_CXX_STANDARD 20)

# Already passed via command line: -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake

find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(main main.cpp)

# Link Raylib and its dependencies
target_link_libraries(main PRIVATE raylib glfw)

