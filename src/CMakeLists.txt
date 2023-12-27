add_library(gakaIO IO/AssetManager.cpp)
add_library(gakaGeometry Geometry/Curves.cpp)
add_library(gakaRendering
    Rendering/Renderer.cpp
    Rendering/RenderObject.cpp
    Rendering/FlyingCamera.cpp
    Rendering/Scene.cpp)
add_library(gakaGPUOpenGL
    GPU/OpenGL/GLShaderProgram.cpp)
add_library(gakaGUIOpenGL
    GUI/SDLWindow.cpp
    GUI/SDLOpenGLWindow.cpp
)

add_executable(gaka_gl main.cpp)

add_dependencies(gaka_gl OpenGLShaders)

target_include_directories(gakaIO PRIVATE ${gaka_include_dir})
target_include_directories(gakaGeometry PRIVATE ${gaka_include_dir})
target_include_directories(gakaRendering PRIVATE ${gaka_include_dir})
target_include_directories(gakaGUIOpenGL PRIVATE ${gaka_include_dir})
target_include_directories(gakaGPUOpenGL PRIVATE ${gaka_include_dir})
target_include_directories(gaka_gl PRIVATE ${gaka_include_dir})

target_link_libraries(gakaRendering PUBLIC gakaIO gakaGeometry OpenGL::OpenGL GLEW glm::glm gakaGPUOpenGL)

target_link_libraries(gakaGUIOpenGL PUBLIC SDL2::SDL2main SDL2::SDL2 OpenGL::OpenGL gakaRendering)

target_link_libraries(gaka_gl PUBLIC gakaGUIOpenGL ${CMAKE_DL_LIBS})

install(TARGETS gaka_gl RUNTIME DESTINATION bin)
