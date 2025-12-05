cmake_minimum_required(VERSION 3.5)

project(tracers)

add_library(${PROJECT_NAME} 
src/path_tracer.cpp)

target_include_directories( 
    ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE core)
target_link_libraries(${PROJECT_NAME} PRIVATE data_structures)
target_link_libraries(${PROJECT_NAME} PRIVATE material)
target_link_libraries(${PROJECT_NAME} PRIVATE primitives)
target_link_libraries(${PROJECT_NAME} PRIVATE structures)

target_compile_options(${PROJECT_NAME} PUBLIC -fopenmp)
target_link_options(${PROJECT_NAME} PUBLIC -fopenmp)