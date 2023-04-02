cmake_minimum_required(VERSION 3.0)

project(gaka)

set(OpenGL_GL_PREFERENCE GLVND)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(gaka 
    src/main.cpp
    src/gl.c
    src/AssetManager.cpp
    src/Rendering/Renderer.cpp
    src/Rendering/RenderObject.cpp
    src/Rendering/ShaderProgram.cpp
    src/Rendering/FlyingCamera.cpp
    src/Rendering/Scene.cpp
    src/Geometry/Curves.cpp
    )

target_include_directories(gaka PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
                           
target_link_libraries(gaka PUBLIC SDL2 OpenGL::GL ${CMAKE_DL_LIBS})

install(TARGETS gaka RUNTIME DESTINATION bin)
