cmake_minimum_required(VERSION 3.10)

project(NanoKit)

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

message(">>> CONFIGURING NANOKIT")

if(WIN32)

    message("    - DETECTED WINDOWS")

    set(PLATFORM_SOURCE
        window/win32_window.c
        app/win32_app.c
    )

    set(PLATFORM_DEPENDENCIES
        user32
        opengl32
    )

elseif(EMSCRIPTEN)
    
    message("    - DETECTED WASM")

    set(PLATFORM_SOURCE
        window/wasm_window.c
        app/wasm_app.c
    )

elseif(DEFINED BUILD_IOS)

    message("    - DETECTED IOS")

    set(PLATFORM_SOURCE
        window/ios_window.m
        app/ios_app.m
    )
    
    set(PLATFORM_DEPENDENCIES
        "-framework Foundation"
        "-framework UIKit"
    )

    add_compile_definitions(GLES_SILENCE_DEPRECATION)


elseif(APPLE)

    message("    - DETECTED MACOS")

    set(PLATFORM_SOURCE
        window/macos_window.m
        app/macos_app.m
    )

    set(PLATFORM_DEPENDENCIES
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework QuartzCore"
    )

    add_compile_definitions(GL_SILENCE_DEPRECATION)
    

elseif(UNIX)

    message("    - DETECTED UNIX")

    set(PLATFORM_SOURCE
        window/unix_window.c
        app/unix_app.c
    )

    set(PLATFORM_DEPENDENCIES
        X11
        GL
        m
    )
else()
    message(FATAL_ERROR "UNSUPPORTED PLATFORM: ${CMAKE_SYSTEM_NAME}")
endif()

add_library(NanoKit STATIC
    ${PLATFORM_SOURCE}
    log/log.c
    app/app.c
    view/view.c
    window/window.c

    library/panels/dockpanel/dockpanel.c
)

target_link_libraries(NanoKit PRIVATE
    ${PLATFORM_DEPENDENCIES}
    NanoKit-Extern
)

target_include_directories(NanoKit PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

embed_resources(NanoKit
    resources/fonts/fredoka.ttf
    resources/fonts/fredoka_semibold.ttf
    resources/fonts/cousine.ttf
    resources/fonts/literata.ttf
    resources/fonts/mrdafoe.ttf
    resources/shaders/shader.frag
    resources/shaders/shader.vert
)

message("<<< DONE")