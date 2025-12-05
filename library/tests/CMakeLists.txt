cmake_minimum_required(VERSION 3.10)

project(tests)

set(CMAKE_CXX_STANDARD 17)

add_executable(vector_test vector_test.cpp)
target_link_libraries(vector_test PRIVATE core)

add_executable(sphere_test sphere_test.cpp)
target_link_libraries(sphere_test PRIVATE core)
target_link_libraries(sphere_test PRIVATE structures)
target_link_libraries(sphere_test PRIVATE primitives)

add_executable(quad_test quad_test.cpp)
target_link_libraries(quad_test PRIVATE core)
target_link_libraries(quad_test PRIVATE structures)
target_link_libraries(quad_test PRIVATE data_structures)
target_link_libraries(quad_test PRIVATE primitives)
target_link_libraries(quad_test PRIVATE material)
target_link_libraries(quad_test PRIVATE tracers)

install(TARGETS sphere_test vector_test quad_test
        DESTINATION "${CMAKE_SOURCE_DIR}/test")