cmake_minimum_required(VERSION 3.20)

project(gaka C CXX)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Path to glad directory
set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/external/glad/")
# Path to glad cmake files
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)
# Specify glad settings
glad_add_library(glad_gl_core_46 REPRODUCIBLE API gl:core=4.6)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(gaka
    src/main.cpp
    src/IO/AssetManager.cpp
    src/Rendering/Renderer.cpp
    src/Rendering/RenderObject.cpp
    src/Rendering/ShaderProgram.cpp
    src/Rendering/FlyingCamera.cpp
    src/Rendering/Scene.cpp
    src/Geometry/Curves.cpp
    )

target_link_libraries(gaka PUBLIC glad_gl_core_46 SDL2 ${CMAKE_DL_LIBS})

install(TARGETS gaka RUNTIME DESTINATION bin)
