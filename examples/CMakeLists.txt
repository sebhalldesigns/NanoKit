cmake_minimum_required(VERSION 3.10)

project(Examples)

add_executable(nanokit_demo main.c)
target_include_directories(nanokit_demo PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(nanokit_demo NanoKit)