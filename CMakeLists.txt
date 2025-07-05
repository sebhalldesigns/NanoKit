cmake_minimum_required(VERSION 3.10)

include(ExternalProject)

if(EMSCRIPTEN OR DEFINED BUILD_IOS)
    ExternalProject_Add(nkgen
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/nkgen
        BINARY_DIR ${CMAKE_BINARY_DIR}/nkgen
        CMAKE_ARGS
            -DCMAKE_OSX_SYSROOT=macosx
            -DCMAKE_SYSTEM_NAME=Darwin
            -DCMAKE_BUILD_TYPE=Release
            -DCMAKE_TOOLCHAIN_FILE=  # empty toolchain = native
            -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}/nkgen
            -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG=${CMAKE_BINARY_DIR}/nkgen
        INSTALL_COMMAND ""  # <- This disables the install step
    )
else()
    add_subdirectory(NKGen)
endif()

add_subdirectory(NanoDraw)
add_subdirectory(NanoWin)

project(NanoKit)

add_library(NanoKit STATIC
    lib/nanokit.c
)

target_include_directories(NanoKit PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/lib
)

target_link_libraries(NanoKit PUBLIC
    #NanoDraw
    NanoWin
)