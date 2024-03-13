add_library(gakaIO IO/RessourceManager.cpp)
add_library(gakaGeometry Geometry/Curves.cpp)
add_library(gakaRendering
    Rendering/Renderer.cpp
    Rendering/Scene.cpp
    Rendering/SceneNodes/CameraNode.cpp
    Rendering/SceneNodes/MeshNode.cpp
    Rendering/SceneNodes/SceneNodes.cpp
    Rendering/SceneNodes/TextureNode.cpp)

target_include_directories(gakaIO PRIVATE ${gaka_include_dir})
target_include_directories(gakaGeometry PRIVATE ${gaka_include_dir})
target_include_directories(gakaRendering PRIVATE ${gaka_include_dir})

add_sanitizers(gakaIO gakaGeometry gakaRendering)

target_link_libraries(gakaIO PUBLIC SAIL::sail-c++)
target_link_libraries(gakaGeometry glm::glm)

add_library(gakaGFX
    GFX/FlyingCamera.cpp
    GFX/Material.cpp
    GFX/MaterialParameters.cpp
)
target_include_directories(gakaGFX PRIVATE ${gaka_include_dir})

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
add_sanitizers(gakaGFXOpenGL)
target_include_directories(gakaGUIOpenGL PRIVATE ${gaka_include_dir})
target_include_directories(gakaGFXOpenGL PRIVATE ${gaka_include_dir})

target_link_libraries(gakaIO PUBLIC SAIL::sail-c++)

target_link_libraries(gakaGeometry glm::glm)

target_link_libraries(gakaGFXOpenGL PUBLIC OpenGL::OpenGL epoxy::epoxy glm::glm)

target_link_libraries(gakaGFX glm::glm gakaGFXOpenGL)

target_link_libraries(gakaRendering PUBLIC gakaIO  gakaGeometry OpenGL::OpenGL glm::glm gakaGFX gakaGFXOpenGL)
target_link_libraries(gakaGUIOpenGL PUBLIC SDL2::SDL2main SDL2::SDL2 OpenGL::OpenGL gakaRendering glm::glm)
