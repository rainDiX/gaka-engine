/*
 * SPDX-License-Identifier: MIT
 */

#include <glm/glm.hpp>
#include <memory>
#include <span>
#include <vector>

#include "GFX/OpenGL/GLMesh.hpp"
#include "Geometry/Mesh.hpp"
#include "Rendering/SceneNodes.hpp"

namespace gk::rendering {

MeshNode::MeshNode(long id, const gk::geometry::Mesh& mesh, MaterialNode* material)
    : SceneNode(id) {
  auto& program = material->program();
  m_mesh = std::make_unique<gfx::gl::Mesh>(std::span<const geometry::Mesh::Vertex>{mesh.vertices},
                                           std::span<const uint>{mesh.indices}, program);
}

MeshNode::MeshNode(long id, const gk::animation::SkinnedMesh& mesh, MaterialNode* material)
    : SceneNode(id) {
  auto& program = material->program();
  m_mesh = std::make_unique<gfx::gl::Mesh>(
      std::span<const animation::SkinnedMesh::Vertex>{mesh.vertices},
      std::span<const uint>{mesh.indices}, program);
}

void MeshNode::draw(const glm::mat4& projection_matrix, const glm::mat4& view_matrix,
                    const glm::vec3& cameraPosition, const std::vector<LightNode*> lights) const {
  m_mesh->bind();
  auto& program = m_material->program();
  program.use();
  program.setUniform("projection", projection_matrix);
  program.setUniform("view", view_matrix);
  program.setUniform("model", m_modelMatrix);
  program.setUniform("view_pos", cameraPosition);

  if (m_params) {
    auto parameters = m_params->parameters();

    for (auto& param : parameters->boolParameters()) {
      program.setUniform(param.first, param.second);
    }
    for (auto& param : parameters->floatParameters()) {
      program.setUniform(param.first, param.second);
    }
    for (auto& param : parameters->intParameters()) {
      program.setUniform(param.first, param.second);
    }
    for (auto& param : parameters->vec2Parameters()) {
      program.setUniform(param.first, param.second);
    }
    for (auto& param : parameters->vec3Parameters()) {
      program.setUniform(param.first, param.second);
    }
    for (auto& param : parameters->vec4Parameters()) {
      program.setUniform(param.first, param.second);
    }
    for (auto& param : parameters->mat3Parameters()) {
      program.setUniform(param.first, param.second);
    }
    for (auto& param : parameters->mat4Parameters()) {
      program.setUniform(param.first, param.second);
    }
  }

  // TODO: better light management
  program.setUniform("nb_point_lights", static_cast<glm::int32>(lights.size()));
  for (int i = 0; i < int(lights.size()); i++) {
    auto& light = lights[i]->light();
    program.setUniform("pointLights[" + std::to_string(i) + "].color", light.color);
    program.setUniform("pointLights[" + std::to_string(i) + "].intensity", light.intensity);
    program.setUniform("pointLights[" + std::to_string(i) + "].range", light.range);
    program.setUniform("pointLights[" + std::to_string(i) + "].decay", light.decay);
    program.setUniform("pointLights[" + std::to_string(i) + "].position", lights[i]->position());
  }

  if (!m_textures.empty()) {
    program.setUniform("hasTex", static_cast<glm::int32>(true));
  }

  for (auto& tex : m_textures) {
    tex->bind();
  }
  m_mesh->draw();
}

void MeshNode::connect(SceneNode* node) noexcept {
  switch (node->nodeType()) {
    case NodeType::eMaterial: {
      auto materialNode = dynamic_cast<MaterialNode*>(node);
      m_material = materialNode;
      m_children.push_back(node);
      break;
    }
    case NodeType::eMaterialParams: {
      auto paramsNode = dynamic_cast<MaterialParameterNode*>(node);
      m_params = paramsNode;
      m_children.push_back(node);
      break;
    }
    case NodeType::eTexture: {
      auto textureNode = dynamic_cast<TextureNode*>(node);
      m_textures.push_back(textureNode);
      m_children.push_back(node);
      break;
    }
    case NodeType::eMesh:
    case NodeType::eCamera:
    case NodeType::eGeneric:
    case NodeType::eLight:
      break;
  }
}

void MeshNode::disconnect(long id) noexcept {
  if (m_material->id() == id) {
    m_material = nullptr;
  } else if (m_params->id() == id) {
    m_params = nullptr;
  }
  auto tex = std::find_if(m_textures.begin(), m_textures.end(),
                          [id](SceneNode* node) { return node->id() == id; });
  if (tex != m_textures.end()) {
    m_textures.erase(tex);
  }
  SceneNode::disconnect(id);
}

void MeshNode::translate(const glm::vec3& translation) noexcept {
  glm::translate(m_modelMatrix, translation);
}

void MeshNode::rotate(float angle, const glm::vec3& axis) noexcept {
  glm::rotate(m_modelMatrix, angle, axis);
}

bool MeshNode::hasMaterial() const noexcept { return m_material != nullptr; }

bool MeshNode::hasTextures() const noexcept { return !m_textures.empty(); }

NodeType MeshNode::nodeType() const { return NodeType::eMesh; }

NodeType MaterialNode::nodeType() const { return NodeType::eMaterial; }

}  // namespace gk::rendering
