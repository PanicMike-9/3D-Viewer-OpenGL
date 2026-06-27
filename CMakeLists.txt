cmake_minimum_required(VERSION 3.10)

project(3dViewerOpenGL)

set(CMAKE_CXX_STANDARD 17)

add_executable(app src/main.cpp 
                   src/glad.c 
                   src/shader.cpp 
                   src/camera.cpp
              )

include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare( 
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.0
)
FetchContent_MakeAvailable(glm)

target_include_directories(app PRIVATE ${glm_SOURCE_DIR})

add_subdirectory(assimp)

target_link_libraries(app
PRIVATE 
opengl32
${CMAKE_SOURCE_DIR}/lib/glfw3.lib
)