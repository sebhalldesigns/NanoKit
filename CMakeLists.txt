cmake_minimum_required(VERSION 3.10)

project(NanoKit)

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

