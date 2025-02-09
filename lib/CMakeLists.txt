cmake_minimum_required(VERSION 3.10)

project(NanoKit)

if(WIN32)
    message(STATUS "Building on Windows")
elseif(APPLE)
    message(STATUS "Building on MacOS")
elseif(UNIX)
    message(STATUS "Building on Linux/Unix")
endif()