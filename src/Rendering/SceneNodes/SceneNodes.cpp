/*
 * SPDX-License-Identifier: MIT
 */

#include "Rendering/SceneNodes.hpp"

#include <algorithm>
#include <glm/fwd.hpp>
#include <memory>

#include "GFX/Material.hpp"
#include "GFX/MaterialParameters.hpp"
#include "GFX/PointLight.hpp"

namespace gk::rendering {

// SceneNode Base class
SceneNode::SceneNode(long id) : m_id(id) {}

long SceneNode::id() const noexcept { return m_id; }

const std::span<SceneNode* const> SceneNode::children() const noexcept { return m_children; }

void SceneNode::connect(SceneNode* node) noexcept { m_children.push_back(node); }

void SceneNode::disconnect(long id) noexcept {
  auto child = std::find_if(m_children.begin(), m_children.end(),
                            [id](SceneNode* node) { return node->id() == id; });
  if (child != m_children.end()) {
    m_children.erase(child);
  }
}

NodeType SceneNode::nodeType() const { return NodeType::eGeneric; }

// LightNode
LightNode::LightNode(long id, gfx::PointLight&& light, const glm::vec3& position) : SceneNode(id) {
  m_light = std::make_unique<gfx::PointLight>(std::move(light));
  m_position = position;
}

NodeType LightNode::nodeType() const { return NodeType::eLight; }

const gfx::PointLight& LightNode::light() const { return *m_light; }

gfx::PointLight& LightNode::light() { return *m_light; }

const glm::vec3& LightNode::position() const { return m_position; }

void LightNode::setPosition(const glm::vec3& position) noexcept { m_position = position; }

// MaterialNode

MaterialNode::MaterialNode(long id, gfx::Material&& material)
    : SceneNode(id), m_material(std::move(material)) {}

gfx::gl::ShaderProgram& MaterialNode::program() const noexcept { return m_material.program(); }

// MaterialParameters
MaterialParameterNode::MaterialParameterNode(long id) : SceneNode(id) {
  m_parameters = std::make_unique<gfx::MockParameters>();
}

MaterialParameterNode::MaterialParameterNode(long id,
                                             std::unique_ptr<gfx::MaterialParameters>&& parameters)
    : SceneNode(id), m_parameters(std::move(parameters)) {}

NodeType MaterialParameterNode::nodeType() const { return NodeType::eMaterialParams; }

const gfx::MaterialParameters* MaterialParameterNode::parameters() const { return m_parameters.get(); }
gfx::MaterialParameters* MaterialParameterNode::parameters() { return m_parameters.get(); }

}  // namespace gk::rendering
