/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <optional>

#include "GFX/Material.hpp"
#include "GFX/PointLight.hpp"
#include "Rendering/SceneNodes.hpp"

namespace gk::rendering {

class Scene {
 public:
  Scene();
  std::optional<CameraNode*> activeCamera() const;
  SceneNode* root() const;
  long rootId() const;
  std::optional<SceneNode*> getNode(long id) const noexcept;
  long addNode() noexcept;
  void setActiveCamera(long id);
  long addCamera(gfx::FlyingCamera&& camera);
  long addLight(gfx::PointLight&& light, const glm::vec3& position);
  long addMaterial(gfx::Material&& material);
  long addMaterialParameter(std::unique_ptr<gfx::MaterialParameters>&& material);
  long addTexture(const std::span<std::byte> texture, int width, int height);
  std::optional<long> addMesh(const gk::geometry::Mesh& mesh, long materialId);
  std::optional<long> addMesh(const gk::animation::SkinnedMesh& mesh, long materialId);
  void connect(long parentId, long childId);

 private:
  SceneNode* m_rootNode;
  long m_counter = 1;
  std::optional<CameraNode*> m_activeCamera;
  std::map<long, std::unique_ptr<SceneNode>> m_nodes{};
};

}  // namespace gk::rendering
