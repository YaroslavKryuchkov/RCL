cmake_minimum_required(VERSION 3.5)

project(data_structures)

add_library(${PROJECT_NAME} 
src/bvh.cpp 
src/photon_map.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE core)
target_link_libraries(${PROJECT_NAME} PRIVATE primitives)
target_link_libraries(${PROJECT_NAME} PRIVATE structures)