cmake_minimum_required(VERSION 3.5)

project(code)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
    rcl
)