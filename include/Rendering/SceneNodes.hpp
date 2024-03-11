/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <span>
#include <vector>

#include "GFX/FlyingCamera.hpp"
#include "GFX/Material.hpp"
#include "GFX/MaterialParameters.hpp"
#include "GFX/OpenGL/GLMesh.hpp"
#include "GFX/OpenGL/GLShaderProgram.hpp"
#include "GFX/OpenGL/GLTexture.hpp"
#include "GFX/PointLight.hpp"
#include "Geometry/Mesh.hpp"

namespace gk::rendering {

enum class NodeType { eGeneric, eMesh, eTexture, eMaterial, eMaterialParams, eCamera, eLight };

class SceneNode {
 public:
  SceneNode(long id);
  SceneNode(const SceneNode&) = delete;
  SceneNode(SceneNode&&) = default;
  SceneNode& operator=(const SceneNode&) = delete;
  SceneNode& operator=(SceneNode&&) = default;

  virtual ~SceneNode() {}
  const std::span<SceneNode* const> children() const noexcept;
  long id() const noexcept;

  virtual NodeType nodeType() const;
  virtual void connect(SceneNode* node) noexcept;
  virtual void disconnect(long id) noexcept;

 protected:
  SceneNode* m_parent = nullptr;
  std::vector<SceneNode*> m_children;

 private:
  long m_id;
};

class LightNode : public SceneNode {
 public:
  LightNode(long id, gfx::PointLight&& light, const glm::vec3& position);
  LightNode(const LightNode&) = delete;
  LightNode(LightNode&&) = default;
  LightNode& operator=(const LightNode&) = delete;
  LightNode& operator=(LightNode&&) = default;

  NodeType nodeType() const override;
  const gfx::PointLight& light() const;
  gfx::PointLight& light();

  const glm::vec3& position() const;
  void setPosition(const glm::vec3& position) noexcept;

 private:
  std::unique_ptr<gfx::PointLight> m_light{};
  glm::vec3 m_position{0};
};

class CameraNode : public SceneNode {
 public:
  CameraNode(long id, gfx::FlyingCamera&& cam);
  CameraNode(const CameraNode&) = delete;
  CameraNode(CameraNode&&) = default;
  CameraNode& operator=(const CameraNode&) = delete;
  CameraNode& operator=(CameraNode&&) = default;

  const gfx::FlyingCamera& camera() const;
  gfx::FlyingCamera& camera();

  NodeType nodeType() const override;

 private:
  SceneNode* parent = nullptr;
  std::unique_ptr<gfx::FlyingCamera> m_camera;
};

class MaterialNode : public SceneNode {
 public:
  MaterialNode(long id, gfx::Material&& material);
  void setParameters() const;
  void setupLights() const;
  NodeType nodeType() const override;

  gfx::gl::ShaderProgram& program() const noexcept;

 protected:
  gfx::Material m_material;
};

class MaterialParameterNode : public SceneNode {
 public:
  MaterialParameterNode(long id);
  MaterialParameterNode(long id, std::unique_ptr<gfx::MaterialParameters>&& material);

  NodeType nodeType() const override;
  const gfx::MaterialParameters* parameters() const;
  gfx::MaterialParameters* parameters();

 private:
  std::unique_ptr<gfx::MaterialParameters> m_parameters;
};

class TextureNode : public SceneNode {
 public:
  TextureNode(long id, const std::span<std::byte> texture, int width, int height);

  TextureNode(const TextureNode&) = delete;

  TextureNode& operator=(const TextureNode&) = delete;

  void update(const std::span<std::byte> texture, int width, int height) noexcept;

  void bind() noexcept;

  NodeType nodeType() const override;

  const gfx::gl::Texture& texture() const noexcept;

 private:
  std::unique_ptr<gfx::gl::Texture> m_tex;
};

class MeshNode : public SceneNode {
 public:
  MeshNode(long id, const gk::geometry::Mesh& mesh, MaterialNode* material);

  MeshNode(const MeshNode&) = delete;

  MeshNode& operator=(const MeshNode&) = delete;

  void connect(SceneNode* node) noexcept override;

  void disconnect(long id) noexcept override;

  void draw(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix,
            const std::vector<LightNode*> lights) const;

  void update(const gk::geometry::Mesh& mesh);

  NodeType nodeType() const override;

  gfx::gl::Mesh& mesh() noexcept;
  std::vector<std::shared_ptr<gfx::gl::Texture>>& textures() noexcept;

  bool hasMaterial() const noexcept;
  bool hasTextures() const noexcept;

  void translate(const glm::vec3& translation) noexcept;
  void rotate(float angle, const glm::vec3& axis) noexcept;

  const gfx::gl::Mesh& mesh() const noexcept;
  const std::span<const std::shared_ptr<gfx::gl::Texture>> textures() const noexcept;

 private:
  std::unique_ptr<gfx::gl::Mesh> m_mesh;
  MaterialNode* m_material = nullptr;
  MaterialParameterNode* m_params = nullptr;
  std::vector<TextureNode*> m_textures;
  glm::mat4 m_modelMatrix = glm::mat4(1.0f);
};

}  // namespace gk::rendering
