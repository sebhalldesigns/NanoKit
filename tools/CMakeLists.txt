cmake_minimum_required(VERSION 3.17)

project(NanoKit-Tools)

if(EMSCRIPTEN)
    include(ExternalProject)
    
    ExternalProject_Add(nkgen
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/nkgen
        BINARY_DIR ${CMAKE_BINARY_DIR}/nkgen
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=Release
            -DCMAKE_TOOLCHAIN_FILE=  # empty toolchain = native
            -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}/nkgen
        INSTALL_COMMAND ""  # <- This disables the install step
    )
else()
    add_subdirectory(nkgen)
endif()