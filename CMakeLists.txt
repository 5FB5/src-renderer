cmake_minimum_required(VERSION 3.25)
project(src_renderer)

include_directories(shader)

add_library(shader shader/shader.cpp)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES main.cpp)

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/test.jpg
        ${CMAKE_CURRENT_BINARY_DIR}/test.jpg)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/test2.jpg
        ${CMAKE_CURRENT_BINARY_DIR}/test2.jpg)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/vertex.glsl
        ${CMAKE_CURRENT_BINARY_DIR}/vertex.glsl)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/fragment.glsl
        ${CMAKE_CURRENT_BINARY_DIR}/fragment.glsl)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/fragment2.glsl
        ${CMAKE_CURRENT_BINARY_DIR}/fragment2.glsl)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/fragment3.glsl
        ${CMAKE_CURRENT_BINARY_DIR}/fragment3.glsl)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE SOIL)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
target_link_libraries(${PROJECT_NAME} PRIVATE shader)
