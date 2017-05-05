cmake_minimum_required(VERSION 3.5)
project(OS_Course_Project)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES DISK_API.cpp Directory.cpp File.cpp main.cpp)
add_executable(OS_Course_Project ${SOURCE_FILES})
