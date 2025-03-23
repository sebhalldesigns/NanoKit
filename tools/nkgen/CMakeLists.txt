cmake_minimum_required(VERSION 3.10)

project(nkgen)

add_executable(nkgen
    src/main.c
    src/xml/xml.c
)

target_include_directories(nkgen PRIVATE
   src/xml
)