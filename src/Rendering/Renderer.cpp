#include "Rendering/Renderer.hpp"

#include <iostream>
#include <memory>
#include <string>

#include "GFX/OpenGL/GLShaderProgram.hpp"
#include "Rendering/FlyingCamera.hpp"
#include "Rendering/RenderObject.hpp"
#include "Rendering/Scene.hpp"

namespace gk::rendering {

Renderer::Renderer(std::shared_ptr<io::RessourceManager> assetManager)
    : m_ressourceManager(assetManager) {
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
  auto line = std::make_shared<gfx::gl::GLShaderProgram>();
  line->compileFile("shaders/OpenGL/mesh.vert", *m_ressourceManager, gfx::gl::ShaderType::VERTEX);
  line->compileFile("shaders/OpenGL/line.frag", *m_ressourceManager, gfx::gl::ShaderType::FRAGMENT);
  line->link();

  auto normal = std::make_shared<gfx::gl::GLShaderProgram>();
  normal->compileFile("shaders/OpenGL/mesh.vert", *m_ressourceManager, gfx::gl::ShaderType::VERTEX);
  normal->compileFile("shaders/OpenGL/normals.frag", *m_ressourceManager, gfx::gl::ShaderType::FRAGMENT);
  normal->link();

  auto parametric = std::make_shared<gfx::gl::GLShaderProgram>();
  parametric->compileFile("shaders/OpenGL/mesh.vert", *m_ressourceManager, gfx::gl::ShaderType::VERTEX);
  parametric->compileFile("shaders/OpenGL/parametric.frag", *m_ressourceManager,
                          gfx::gl::ShaderType::FRAGMENT);
  parametric->link();

  auto lambertian = std::make_shared<gfx::gl::GLShaderProgram>();
  lambertian->compileFile("shaders/OpenGL/mesh.vert", *m_ressourceManager, gfx::gl::ShaderType::VERTEX);
  lambertian->compileFile("shaders/OpenGL/lambertian.frag", *m_ressourceManager,
                          gfx::gl::ShaderType::FRAGMENT);
  lambertian->link();

  auto phong = std::make_shared<gfx::gl::GLShaderProgram>();
  phong->compileFile("shaders/OpenGL/mesh.vert", *m_ressourceManager, gfx::gl::ShaderType::VERTEX);
  phong->compileFile("shaders/OpenGL/phong.frag", *m_ressourceManager, gfx::gl::ShaderType::FRAGMENT);
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

const std::shared_ptr<gfx::gl::GLShaderProgram> Renderer::getProgram(
    const std::string& name) const {
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
