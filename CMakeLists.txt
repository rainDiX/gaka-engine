cmake_minimum_required(VERSION 3.20)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/externals/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH})

project(gaka C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable more warnings and disable C++20 char8_t
if (MSVC)
  add_compile_options(/W4 /Zc:char8_t-)
else ()
  add_compile_options(-Wall -Wextra -Wpedantic -fno-char8_t)
endif()

# Enable Sanitizers for dev build
if(CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
  set(SANITIZE_ADDRESS ON)
  set(SANITIZE_MEMORY OFF)
  set(SANITIZE_THREAD OFF)
  set(SANITIZE_UNDEFINED ON)
endif()
find_package(Sanitizers)

find_package(SDL2 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(SailC++ CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(epoxy REQUIRED)

set(gaka_include_dir "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(shaders)
add_subdirectory(src)
