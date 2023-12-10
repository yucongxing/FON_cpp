cmake_minimum_required(VERSION 3.25)

project(FocusOnNow LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "CMAKE_BUILD_TYPE not given, default use Release!")
endif()

message("---${CMAKE_BUILD_TYPE}---")

add_subdirectory(src)

option(BUILD_TEST "build test." OFF)
if(BUILD_TEST)
    add_subdirectory(test)
endif()
