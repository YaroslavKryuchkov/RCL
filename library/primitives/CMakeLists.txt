cmake_minimum_required(VERSION 3.5)

project(primitives)

add_library(${PROJECT_NAME} 
src/hittable_list.cpp
src/mesh.cpp
src/quad.cpp
src/sphere.cpp
src/vertex_triangle.cpp
src/model_workers.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE core)
target_link_libraries(${PROJECT_NAME} PRIVATE structures)
target_link_libraries(${PROJECT_NAME} PRIVATE material)
target_link_libraries(${PROJECT_NAME} PRIVATE data_structures)