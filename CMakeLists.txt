cmake_minimum_required(VERSION 3.10)
project(quasar_simulation)

set(CMAKE_CXX_STANDARD_17)
set(OpenGL_GL_PREFERENCE GLVND)


find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(include)


add_executable(quasar_sim src/glad.c src/main.cpp src/gfx/camera.cpp)


target_link_libraries(quasar_sim OpenGL::GL glfw dl)

target_include_directories(quasar_sim PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src

    
)
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
