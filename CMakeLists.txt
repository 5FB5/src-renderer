cmake_minimum_required(VERSION 3.25)
project(src_renderer)

include_directories(shader)

add_library(shader shader/shader.cpp)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES main.cpp)

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
target_link_libraries(${PROJECT_NAME} PRIVATE shader)
