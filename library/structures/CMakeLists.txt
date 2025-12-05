cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(structures)

add_library(${PROJECT_NAME} 
src/aabb.cpp 
src/camera.cpp
src/picture.cpp
src/ray.cpp
src/pictures_workers.cpp)

target_include_directories( 
    ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE core)