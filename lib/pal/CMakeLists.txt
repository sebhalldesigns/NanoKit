cmake_minimum_required(VERSION 3.10)

project(NanoKit-PAL)

message(">>> CONFIGURING NANOKIT-PAL")

if(WIN32)

    message("    - DETECTED WINDOWS")

    set(PLATFORM_SOURCE
        win32/thread.c
        win32/window.c
        win32/event.c
    )

    set(PLATFORM_DEPENDENCIES
        user32
        opengl32
    )

elseif(EMSCRIPTEN)
    
    message("    - DETECTED WASM")

elseif(UNIX)

    message("    - DETECTED UNIX")

    set(PLATFORM_SOURCE
        pal/unix/thread.c
        pal/unix/window.c
        pal/unix/event.c

        extern/glad/glad.c
        extern/nanovg/src/nanovg.c
    )

    set(PLATFORM_DEPENDENCIES
        X11
        GL
        m
    )
else()
    message(FATAL_ERROR "UNSUPPORTED PLATFORM: ${CMAKE_SYSTEM_NAME}")
endif()

add_library(NanoKit-PAL STATIC
    ${PLATFORM_SOURCE}
)

target_link_libraries(NanoKit-PAL 
    ${PLATFORM_DEPENDENCIES}
    NanoKit-Extern
)

target_include_directories(NanoKit-PAL PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

message("<<< DONE")