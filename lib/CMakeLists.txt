cmake_minimum_required(VERSION 3.10)

project(NanoKit)

if(WIN32)

    message(">>> NANOKIT BUILD FOR WINDOWS")

    set(PLATFORM_SOURCE
        pal/win32/thread.c
        pal/win32/window.c
        pal/win32/event.c

        extern/glad/glad.c
        extern/nanovg/src/nanovg.c
    )

    set(PLATFORM_DEPENDENCIES
        user32
        opengl32
    )
elseif(UNIX)

    message(">>> NANOKIT BUILD FOR UNIX")

    set(PLATFORM_SOURCE
        pal/unix/thread.c
    )

endif()

add_library(NanoKit STATIC
    ${PLATFORM_SOURCE}
    #kit/threadpool/threadpool.c
    log/log.c
)

target_link_libraries(NanoKit ${PLATFORM_DEPENDENCIES} assimp)

target_include_directories(NanoKit PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    
)
