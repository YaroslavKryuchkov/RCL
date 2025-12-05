cmake_minimum_required(VERSION 3.5)

project(material)

add_library(${PROJECT_NAME} 
src/materials.cpp 
src/picture_texture.cpp
src/solid_color.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE core)
target_link_libraries(${PROJECT_NAME} PRIVATE structures)