#include "Rendering/Renderer.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "GPU/OpenGL/GLShaderProgram.hpp"
#include "Rendering/FlyingCamera.hpp"
#include "Rendering/RenderObject.hpp"
#include "Rendering/Scene.hpp"

namespace gk::rendering {

Renderer::Renderer(std::shared_ptr<io::AssetManager> assetManager) : m_asset_manager(assetManager) {
    auto err = glewInit();
    if (err != GLEW_OK && err != GLEW_ERROR_NO_GLX_DISPLAY) {
        std::cout << glewGetErrorString(err) << std::endl;
        throw std::runtime_error("Failed to initialize OpenGL context");
    }

    std::cerr << "Loaded OpenGL " << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // m_scene = Scene();
    m_aspectRatio = 0.0f;
    compileShaders();
}

// temporary
void Renderer::compileShaders() {
    auto line = std::make_shared<gpu::gl::GLShaderProgram>();
    line->compileFile("shaders/mesh.vert", *m_asset_manager, gpu::gl::ShaderType::Vertex);
    line->compileFile("shaders/line.frag", *m_asset_manager, gpu::gl::ShaderType::Fragment);
    line->link();

    auto normal = std::make_shared<gpu::gl::GLShaderProgram>();
    normal->compileFile("shaders/mesh.vert", *m_asset_manager, gpu::gl::ShaderType::Vertex);
    normal->compileFile("shaders/normals.frag", *m_asset_manager, gpu::gl::ShaderType::Fragment);
    normal->link();

    auto parametric = std::make_shared<gpu::gl::GLShaderProgram>();
    parametric->compileFile("shaders/mesh.vert", *m_asset_manager, gpu::gl::ShaderType::Vertex);
    parametric->compileFile("shaders/parametric.frag", *m_asset_manager, gpu::gl::ShaderType::Fragment);
    parametric->link();

    auto lambertian = std::make_shared<gpu::gl::GLShaderProgram>();
    std::cerr << lambertian->id() << '\n';
    lambertian->compileFile("shaders/mesh.vert", *m_asset_manager, gpu::gl::ShaderType::Vertex);
    lambertian->compileFile("shaders/lambertian.frag", *m_asset_manager, gpu::gl::ShaderType::Fragment);
    lambertian->link();

    auto phong = std::make_shared<gpu::gl::GLShaderProgram>();
    phong->compileFile("shaders/mesh.vert", *m_asset_manager, gpu::gl::ShaderType::Vertex);
    phong->compileFile("shaders/phong.frag", *m_asset_manager, gpu::gl::ShaderType::Fragment);
    phong->link();

    m_programs.insert(std::make_pair("line", line));
    m_programs.insert(std::make_pair("normal", normal));
    m_programs.insert(std::make_pair("lambertian", lambertian));
    m_programs.insert(std::make_pair("phong", lambertian));
}

void Renderer::renderScene() const {
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto camera = m_scene.camera();
    glm::mat4 projection = glm::perspective(glm::radians(camera.fov()), m_aspectRatio, 0.5f, 1000.0f);
    glm::mat4 model = glm::mat4(1.0f);
    // TODO for now the modelMatrix is just the identity
    // model = glm::rotate(&model, glm::radians(angle), glm::vec3(1.0f, 0.3f,
    // 0.5f)); model = glm::translate(&model, glm::vec3(0.0f, 0.0f, 0.0f));
    for (auto& objectPair : m_scene.objects()) {
        auto& object = objectPair.second;
        object->draw(projection, camera.getViewMatrix(), model, m_scene.pointLights());
    }
}

Scene& Renderer::getScene() { return m_scene; }

const Scene& Renderer::getScene() const { return m_scene; }

const std::shared_ptr<gpu::gl::GLShaderProgram> Renderer::getProgram(const std::string& name) const {
    auto it = m_programs.find(name);
    if (it != m_programs.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

void Renderer::setViewport(int x, int y, int width, int height) {
    m_aspectRatio = float(width) / float(height);
    glViewport(x, y, width, height);
}

void Renderer::resize(int width, int height) {
    m_aspectRatio = float(width) / float(height);
    glViewport(0, 0, width, height);
}

}  // namespace gk::rendering
