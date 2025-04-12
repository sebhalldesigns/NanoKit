cmake_minimum_required(VERSION 3.17)

project(NanoKit)

if(MSVC)
    set(CMAKE_GENERATOR_PLATFORM x64)  # or x86, ARM, etc.
endif()

add_subdirectory(tools)
add_subdirectory(lib)

option(BUILD_EXAMPLE "Build the Example executable" OFF)

if(BUILD_EXAMPLE)
    add_executable(Example
        examples/main.c
    )

    target_include_directories(Example PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
    )

    target_link_libraries(Example NanoKit)
endif()

