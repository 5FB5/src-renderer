cmake_minimum_required(VERSION 3.25)
project(src_renderer)

include_directories(libs)
include_directories(libs/glm/glm)
include_directories(libs/shader)

include_directories(shaders)
include_directories(resources)

add_library(shadersCpp libs/shader/shader.cpp)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES main.cpp)

find_package(glfw REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/test.jpg
        ${CMAKE_CURRENT_BINARY_DIR}/resources/test.jpg)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/test2.jpg
        ${CMAKE_CURRENT_BINARY_DIR}/resources/test2.jpg)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/shaders/vertex.glsl
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/vertex.glsl)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/shaders/fragment.glsl
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/fragment.glsl)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/shaders/fragment2.glsl
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/fragment2.glsl)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/shaders/fragment3.glsl
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/fragment3.glsl)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE SOIL)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
target_link_libraries(${PROJECT_NAME} PRIVATE shadersCpp)
