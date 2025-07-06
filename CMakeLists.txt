cmake_minimum_required(VERSION 3.10)

# Embed Resources function courtesy of shir0areed on GitHub
function(embed_resources target)
    set(script_path "${CMAKE_CURRENT_BINARY_DIR}/anything_to_c.cmake")
    file(WRITE  "${script_path}" "file(READ \${CMAKE_ARGV3} buf HEX)\n")
    file(APPEND "${script_path}" "string(REGEX REPLACE \"([0-9a-f][0-9a-f])\" \"0x\\\\1, \" buf \${buf})\n")
    file(APPEND "${script_path}" "file(WRITE \${CMAKE_ARGV4} \"const unsigned char \${CMAKE_ARGV5}[] = { \${buf}0x00 };\\n\")\n")
    file(APPEND "${script_path}" "file(APPEND \${CMAKE_ARGV4} \"const unsigned \${CMAKE_ARGV6} = sizeof(\${CMAKE_ARGV5}) - 1;\\n\")\n")
    foreach(res_path ${ARGN})
        string(MAKE_C_IDENTIFIER ${res_path} identifier)
        set(src_path "${CMAKE_CURRENT_SOURCE_DIR}/${res_path}")
        set(dst_path "${CMAKE_CURRENT_BINARY_DIR}/${identifier}.c")
        set(anything_to_c ${CMAKE_COMMAND} -P ${script_path} ${src_path} ${dst_path} ${identifier} ${identifier}_size)
        add_custom_command(OUTPUT ${dst_path} COMMAND ${anything_to_c} DEPENDS ${src_path} VERBATIM)
        target_sources(${target} PRIVATE ${src_path} ${dst_path} )
    endforeach()
endfunction()

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
    NanoDraw
    NanoWin
)

embed_resources(NanoKit
    NKFonts/fonts/Roboto-Regular.ttf
)