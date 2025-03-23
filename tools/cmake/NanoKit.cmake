# NanoKit/tools/cmake/NanoKit.cmake

function(generate_modules target)

    # Get the list of modules passed to the function
    set(modules ${ARGN})
    
    # Create a directory for generated files
    set(GEN_DIR "${CMAKE_BINARY_DIR}/generated")
    file(MAKE_DIRECTORY "${GEN_DIR}")
    
    # Get the path to the nkgen executable
    set(NKGEN "$<TARGET_FILE:nkgen>")

    foreach(mod ${modules})
        # Get the base name: if mod is "src/Window", then mod_base becomes "Window"
        get_filename_component(mod_base ${mod} NAME)

        # Convert mod_base to uppercase
        string(TOUPPER ${mod_base} mod_base_upper)
        
        # Form full paths for the input files (we append the extension)
        set(xml_file "${CMAKE_CURRENT_SOURCE_DIR}/${mod}.xml")
        set(src_file "${CMAKE_CURRENT_SOURCE_DIR}/${mod}.c")
        
        # Generated file paths (we expect nkgen to produce these names)
        set(gen_header "${GEN_DIR}/${mod_base}.xml.h")
        set(gen_src    "${GEN_DIR}/${mod_base}.xml.c")
        
        add_custom_command(
            OUTPUT ${gen_header} ${gen_src}  # These files are the output of the custom command
            COMMAND ${NKGEN} ${mod_base} ${xml_file} ${gen_header} ${gen_src}
            COMMENT "RUNNING NKGEN ${mod_base} ${xml_file} ${gen_header} ${gen_src}"
            DEPENDS ${xml_file}             # nkgen depends on the .xml file
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            VERBATIM
        )

        # add the module as a static library
        add_library(${mod_base} STATIC 
            ${gen_header} 
            ${gen_src}
            ${src_file}
        )

        target_compile_definitions(${mod_base} PUBLIC "${mod_base_upper}_BUILD")
        target_include_directories(${mod_base} PUBLIC ${GEN_DIR})
        target_link_libraries(${target} ${mod_base})
        
    endforeach()

endfunction()

function(generate_html)

    # if(EMSCRIPTEN)

        # path to the html file
        set(INPUT_FILE "${CMAKE_SOURCE_DIR}/NanoKit/lib/resources/index.html")

        # path to the output file
        set(OUTPUT_FILE "${CMAKE_SOURCE_DIR}/build/index.html")

        # Read the original file
        file(READ ${INPUT_FILE} FILE_CONTENTS)

        # Replace a placeholder string with the project or target name
        string(REPLACE "@TARGET_NAME@" "${CMAKE_PROJECT_NAME}" FILE_CONTENTS "${FILE_CONTENTS}")

        # Write the modified content to a new file
        file(WRITE ${OUTPUT_FILE} "${FILE_CONTENTS}")
    # endif()

endfunction()