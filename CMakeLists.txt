cmake_minimum_required(VERSION 3.25)
project(src_renderer)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES main.cpp)

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} GLEW)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

