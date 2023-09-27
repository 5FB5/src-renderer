cmake_minimum_required(VERSION 3.25)
project(src_renderer)

include_directories(libs/glm/glm)
include_directories(libs/shader)
include_directories(libs/camera)
include_directories(libs/valve)

set(CMAKE_CXX_STANDARD 17)

set(HEADERS libs/valve/bsp.h
            libs/shader/shader.h
            libs/camera/camera.h)
set(SOURCES main.cpp
            libs/shader/shader.cpp
            libs/camera/camera.cpp
            libs/valve/bsp.cpp)

find_package(glfw REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

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
        ${CMAKE_SOURCE_DIR}/shaders/vertex.vert
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/vertex.vert)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/shaders/vertexNoNormals.vert
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/vertexNoNormals.vert)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/shaders/color.frag
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/color.frag)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/shaders/light.frag
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/light.frag)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE SOIL)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)