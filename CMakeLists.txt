cmake_minimum_required(VERSION 3.16)
project("project")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

# Collect all .cpp and .c files in src/
file(GLOB SRCS "./src/*.cpp" "./src/*.c")

# Optionally, print the collected source files for debugging
message(STATUS "Source files: ${SRCS}")

add_executable(project ${SRCS})
target_include_directories(project PRIVATE "./include")


