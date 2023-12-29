/*
 * SPDX-License-Identifier: MIT
 */

#include "Rendering/RenderObject.hpp"

#include <memory>
#include <span>
#include <vector>

#include "GFX/OpenGL/GLMesh.hpp"
#include "Geometry/Mesh.hpp"

namespace gk::rendering {

RenderObject::RenderObject(const gk::geometry::Mesh& mesh,
                           const std::shared_ptr<gfx::gl::GLShaderProgram> program,
                           const std::vector<gfx::gl::GLTexture>& textures,
                           const std::shared_ptr<Material> material)
    : m_program(program), m_material(material), m_textures(textures) {
  m_mesh = std::make_unique<gfx::gl::GLMesh>(std::span<const geometry::Vertex>{mesh.vertices},
                                             mesh.indices, m_program.get());
}

void RenderObject::draw(const glm::mat4& projection_matrix, const glm::mat4& view_matrix,
                        const glm::mat4& model_matrix,
                        const std::vector<std::pair<PointLight, glm::vec3>>& lights) const {
  m_mesh->bind();
  m_program->use();
  m_program->setUniform("projection", &projection_matrix);
  m_program->setUniform("view", &view_matrix);
  m_program->setUniform("model", &model_matrix);

  m_program->setUniform<const glm::vec3*>("material.ambient", &(m_material->ambient));
  m_program->setUniform<const glm::vec3*>("material.diffuse", &m_material->diffuse);
  m_program->setUniform<const glm::vec3*>("material.specular", &m_material->specular);
  m_program->setUniform("material.shininess", m_material->shininess);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  m_program->setUniform("nb_point_lights", static_cast<GLint>(lights.size()));
  for (int i = 0; i < int(lights.size()); i++) {
    m_program->setUniform("pointLights[" + std::to_string(i) + "].color", &lights[i].first.color);
    m_program->setUniform("pointLights[" + std::to_string(i) + "].intensity",
                          lights[i].first.intensity);
    m_program->setUniform("pointLights[" + std::to_string(i) + "].range", lights[i].first.range);
    m_program->setUniform("pointLights[" + std::to_string(i) + "].decay", lights[i].first.decay);
    m_program->setUniform("pointLights[" + std::to_string(i) + "].position", &lights[i].second);
  }
  m_mesh->draw();
}

gfx::gl::GLMesh& RenderObject::mesh() noexcept { return *m_mesh; }
const gfx::gl::GLMesh& RenderObject::mesh() const noexcept { return *m_mesh; }

gfx::gl::GLShaderProgram& RenderObject::program() noexcept { return *m_program; }
const gfx::gl::GLShaderProgram& RenderObject::program() const noexcept { return *m_program; }

Material& RenderObject::material() noexcept { return *m_material; }
const Material& RenderObject::material() const noexcept { return *m_material; }

std::vector<gfx::gl::GLTexture>& RenderObject::textures() noexcept { return m_textures; }
const std::span<const gfx::gl::GLTexture> RenderObject::textures() const noexcept {
  return m_textures;
}

}  // namespace gk::rendering
