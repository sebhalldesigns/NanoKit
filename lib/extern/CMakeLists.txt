cmake_minimum_required(VERSION 3.10)

project(NanoKit-Extern)

message(">>> CONFIGURING NANOKIT-EXTERN")

add_library(NanoKit-Extern STATIC
    glad/glad.c
    nanovg/src/nanovg.c
)

target_include_directories(NanoKit-Extern PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}   
)

message("<<< DONE")

