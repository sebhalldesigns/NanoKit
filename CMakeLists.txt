cmake_minimum_required(VERSION 3.10)

add_subdirectory(lib)

project(NanoKit)

add_executable(Example
    examples/main.c
)

target_include_directories(Example PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

target_link_libraries(Example NanoKit)

