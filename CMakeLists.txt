cmake_minimum_required(VERSION 3.10)

# Set the project name
project(NanoKit)

# Add subdirectory
add_subdirectory(Components/NanoDraw)
add_subdirectory(Components/NanoWin)
add_subdirectory(Components/NanoGraph)

# Add an executable
add_executable(NanoKit 
    Test/main.c
)

target_include_directories(NanoKit PRIVATE
    Components/NanoDraw
    Components/NanoWin
    Components/NanoGraph
)

target_link_libraries(NanoKit PRIVATE 
    NanoDraw
    NanoWin
    NanoGraph
)