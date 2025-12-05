cmake_minimum_required(VERSION 3.5)

project(rcl)

add_subdirectory(core)
add_subdirectory(data_structures)
add_subdirectory(material)
add_subdirectory(primitives)
add_subdirectory(structures)
add_subdirectory(tracers)
add_subdirectory(tests)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE
    tracers
    structures
    primitives
    core
    data_structures
    material
)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/../include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
)
