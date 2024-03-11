#include "Rendering/Renderer.hpp"

#include <iostream>
#include <memory>
#include <vector>

#include "GFX/FlyingCamera.hpp"
#include "Rendering/Scene.hpp"
#include "Rendering/SceneNodes.hpp"

namespace gk::rendering {

Renderer::Renderer(std::shared_ptr<io::RessourceManager> assetManager)
    : m_ressourceManager(assetManager) {
  std::cerr << "Loaded OpenGL " << glGetString(GL_VERSION) << std::endl;

  glEnable(GL_MULTISAMPLE);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  m_scene = Scene();
  m_aspectRatio = 0.0f;
}

std::vector<LightNode*> getLights(SceneNode* node) {
  std::vector<LightNode*> lights;
  for (auto child : node->children()) {
    if (child->nodeType() == NodeType::eLight) {
      auto lightnode = dynamic_cast<LightNode*>(child);
      lights.push_back(lightnode);
    }
  }
  return lights;
}

void Renderer::renderNode(SceneNode* node, const glm::mat4& projection,
                          const gfx::FlyingCamera& camera, std::vector<LightNode*> lights) const {
  switch (node->nodeType()) {
    case NodeType::eGeneric: {
      auto groupLights = getLights(node);
      lights.insert(lights.end(), groupLights.begin(), groupLights.end());
      for (auto child : node->children()) {
        renderNode(child, projection, camera, lights);
      }
      break;
    }
    case NodeType::eMesh: {
      auto mesh = dynamic_cast<MeshNode*>(node);
      mesh->draw(projection, camera.getViewMatrix(), lights);
      break;
    }
    default:
      break;
  }
}

void Renderer::renderScene() const {
  glClearColor(0.1, 0.1, 0.1, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  auto activeCam = m_scene.activeCamera();
  if (activeCam.has_value()) {
    auto camera = (*activeCam)->camera();
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.fov()), m_aspectRatio, 0.5f, 1000.0f);
    renderNode(m_scene.root(), projection, camera);
  }
}

Scene& Renderer::getScene() { return m_scene; }

const Scene& Renderer::getScene() const { return m_scene; }

void Renderer::setViewport(int x, int y, int width, int height) {
  m_aspectRatio = float(width) / float(height);
  glViewport(x, y, width, height);
}

void Renderer::resize(int width, int height) {
  m_aspectRatio = float(width) / float(height);
  glViewport(0, 0, width, height);
}

}  // namespace gk::rendering
