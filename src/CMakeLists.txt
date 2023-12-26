add_library(gakaIO IO/AssetManager.cpp)
add_library(gakaGeometry Geometry/Curves.cpp)
add_library(gakaRendering
    Rendering/Renderer.cpp
    Rendering/RenderObject.cpp
    Rendering/ShaderProgram.cpp
    Rendering/FlyingCamera.cpp
    Rendering/Scene.cpp)


add_executable(gaka_gl main.cpp)


target_include_directories(gakaIO PRIVATE ${gaka_include_dir})
target_include_directories(gakaGeometry PRIVATE ${gaka_include_dir})
target_include_directories(gakaRendering PRIVATE ${gaka_include_dir})
target_include_directories(gaka_gl PRIVATE ${gaka_include_dir})

target_link_libraries(gakaRendering PUBLIC gakaIO gakaGeometry OpenGL::OpenGL GLEW glm::glm)

target_link_libraries(gaka_gl PUBLIC SDL2::SDL2main SDL2::SDL2 gakaRendering ${CMAKE_DL_LIBS})

install(TARGETS gaka_gl RUNTIME DESTINATION bin)
