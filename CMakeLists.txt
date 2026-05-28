cmake_minimum_required(VERSION 3.10)

project(3dViewerOpenGL)

set(CMAKE_CXX_STANDARD 17)

add_executable(app src/main.cpp src/glad.c)

include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(app
opengl32
${CMAKE_SOURCE_DIR}/lib/glfw3.lib
)