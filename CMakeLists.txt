cmake_minimum_required(VERSION 3.20)

project(gaka C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

if (MSVC)
  add_compile_options(/W4 /Zc:char8_t-)
else ()
  add_compile_options(-Wall -Wextra -Wpedantic -fno-char8_t)
endif()

set(gaka_include_dir "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(src)
