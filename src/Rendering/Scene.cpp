/*
 * SPDX-License-Identifier: MIT
 */

#include "Rendering/Scene.hpp"

#include <memory>
#include <utility>

#include "GFX/FlyingCamera.hpp"
#include "GFX/PointLight.hpp"
#include "Rendering/SceneNodes.hpp"

namespace gk::rendering {
Scene::Scene() {
  auto rootNode = std::make_unique<SceneNode>(0);
  m_rootNode = rootNode.get();
  m_nodes[0] = std::move(rootNode);
}

std::optional<SceneNode*> Scene::getNode(long id) const noexcept {
  if (auto node = m_nodes.find(id); node != m_nodes.end()) {
    return node->second.get();
  }
  return {};
}

std::optional<CameraNode*> Scene::activeCamera() const { return m_activeCamera; }

void Scene::setActiveCamera(long id) {
  if (auto node = getNode(id); node.has_value()) {
    auto camera = dynamic_cast<CameraNode*>(*node);
    if (camera) {
      m_activeCamera = camera;
    }
  }
}

long Scene::addNode() noexcept {
  auto node = std::make_unique<SceneNode>(m_counter);
  m_nodes[m_counter] = std::move(node);
  return m_counter++;
}

long Scene::addCamera(gfx::FlyingCamera&& camera) {
  auto node = std::make_unique<CameraNode>(m_counter, std::move(camera));
  m_nodes[m_counter] = std::move(node);
  return m_counter++;
}

long Scene::addLight(gfx::PointLight&& light, const glm::vec3& position) {
  auto node = std::make_unique<LightNode>(m_counter, std::move(light), position);
  m_nodes[m_counter] = std::move(node);
  return m_counter++;
}

long Scene::addMaterial(gfx::Material&& material) {
  auto node = std::make_unique<MaterialNode>(m_counter, std::move(material));
  m_nodes[m_counter] = std::move(node);
  return m_counter++;
}

long Scene::addMaterialParameter(std::unique_ptr<gfx::MaterialParameters>&& material) {
  auto node = std::make_unique<MaterialParameterNode>(m_counter, std::move(material));
  m_nodes[m_counter] = std::move(node);
  return m_counter++;
}

long Scene::addTexture(const std::span<std::byte> texture, int width, int height) {
  auto texNode = std::make_unique<TextureNode>(m_counter, texture, width, height);
  m_nodes[m_counter] = std::move(texNode);
  return m_counter++;
}

std::optional<long> Scene::addMesh(const gk::geometry::Mesh& mesh, long materialId) {
  auto materialNode = getNode(materialId);
  if (materialNode.has_value()) {
    auto material = dynamic_cast<MaterialNode*>(*materialNode);
    if (material) {
      auto meshNode = std::make_unique<MeshNode>(m_counter, mesh, material);
      m_nodes[m_counter] = std::move(meshNode);
      return m_counter++;
    }
  }
  return {};
}

void Scene::connect(long parentId, long childId) {
  auto parent = getNode(parentId);
  auto child = getNode(childId);

  if (parent.has_value() && child.has_value()) {
    (*parent)->connect(*child);
  }
}


SceneNode* Scene::root() const { return m_rootNode; }
long Scene::rootId() const { return m_rootNode->id(); }

}  // namespace gk::rendering
