cmake_minimum_required(VERSION 3.5)

project(grass LANGUAGES CXX)

add_subdirectory(library)
add_subdirectory(code)

target_compile_options(grass PRIVATE -fopenmp)
target_link_options(grass PRIVATE -fopenmp)

install(TARGETS grass
        DESTINATION "${CMAKE_SOURCE_DIR}/project")

install(TARGETS rcl
        DESTINATION "${CMAKE_SOURCE_DIR}/project/libs")