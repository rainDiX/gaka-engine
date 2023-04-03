#include "Renderer.hpp"

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "FlyingCamera.hpp"
#include "RenderObject.hpp"
#include "Scene.hpp"
#include "ShaderProgram.hpp"

Renderer::Renderer(void* (*loadfunc)(const char*), std::shared_ptr<AssetManager> assetManager)
    : m_asset_manager(assetManager) {
    int version = gladLoadGL((GLADloadfunc)loadfunc);
    if (version == 0) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        throw -1;
    }

    std::cerr << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // m_scene = Scene();
    m_aspectRatio = 0.0f;
    compileShaders();
}

// temporary
void Renderer::compileShaders() {
    auto normal = std::make_shared<ShaderProgram>();
    normal->compileFile("shaders/mesh.vert", *m_asset_manager, ShaderType::Vertex);
    normal->compileFile("shaders/normals.frag", *m_asset_manager, ShaderType::Fragment);
    normal->link();
    auto phong = std::make_shared<ShaderProgram>();
    phong->compileFile("shaders/mesh.vert", *m_asset_manager, ShaderType::Vertex);
    phong->compileFile("shaders/phong.frag", *m_asset_manager, ShaderType::Fragment);
    phong->link();

    m_programs.insert(std::make_pair("normal", normal));
    m_programs.insert(std::make_pair("phong", phong));
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

const std::shared_ptr<ShaderProgram> Renderer::getProgram(const std::string& name) const {
    auto it = m_programs.find(name);
    if (it != m_programs.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

std::optional<std::unique_ptr<RenderObject>> Renderer::createObject(const Mesh& mesh, const std::string& programName,
                                                                    const std::vector<Texture>& textures,
                                                                    const std::shared_ptr<Material> material) const {
    auto program = getProgram(programName);
    if (program != nullptr) {
        return {std::make_unique<RenderObject>(mesh, program, textures, material)};
    }
    return std::nullopt;
}

void Renderer::setViewport(int x, int y, int width, int height) {
    m_aspectRatio = float(width) / float(height);
    glViewport(x, y, width, height);
}

void Renderer::resize(int width, int height) {
    m_aspectRatio = float(width) / float(height);
    glViewport(0, 0, width, height);
}