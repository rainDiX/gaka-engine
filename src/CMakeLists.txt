add_library(gakaIO IO/RessourceManager.cpp)
add_library(gakaGeometry Geometry/Curves.cpp)
add_library(gakaRendering
    Rendering/Renderer.cpp
    Rendering/RenderObject.cpp
    Rendering/FlyingCamera.cpp
    Rendering/Scene.cpp)

target_include_directories(gakaIO PRIVATE ${gaka_include_dir})
target_include_directories(gakaGeometry PRIVATE ${gaka_include_dir})
target_include_directories(gakaRendering PRIVATE ${gaka_include_dir})

add_sanitizers(gakaIO gakaGeometry gakaRendering)

add_library(gakaGFXOpenGL
    GFX/OpenGL/GLHelperFn.cpp
    GFX/OpenGL/GLShaderProgram.cpp
    GFX/OpenGL/GLMesh.cpp
    GFX/OpenGL/GLTexture.cpp)
add_library(gakaGUIOpenGL
    GUI/SDLWindow.cpp
    GUI/SDLOpenGLWindow.cpp
)
add_dependencies(gakaGFXOpenGL OpenGLShaders)
add_executable(gaka_gl main.cpp)

add_sanitizers(gakaGFXOpenGL gaka_gl)

target_include_directories(gakaGUIOpenGL PRIVATE ${gaka_include_dir})
target_include_directories(gakaGFXOpenGL PRIVATE ${gaka_include_dir})
target_include_directories(gaka_gl PRIVATE ${gaka_include_dir})

target_link_libraries(gakaIO PUBLIC SAIL::sail-c++)
target_link_libraries(gakaGeometry glm::glm)
target_link_libraries(gakaGFXOpenGL PUBLIC OpenGL::OpenGL libepoxy glm::glm SAIL::sail-c++)
target_link_libraries(gakaRendering PUBLIC gakaIO gakaGeometry OpenGL::OpenGL libepoxy glm::glm gakaGFXOpenGL)
target_link_libraries(gakaGUIOpenGL PUBLIC SDL2::SDL2main SDL2::SDL2 OpenGL::OpenGL libepoxy gakaRendering glm::glm)
target_link_libraries(gaka_gl PUBLIC gakaGUIOpenGL ${CMAKE_DL_LIBS})

install(TARGETS gaka_gl RUNTIME DESTINATION bin)
