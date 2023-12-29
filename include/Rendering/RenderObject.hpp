/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <epoxy/gl.h>

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "GFX/OpenGL/GLMesh.hpp"
#include "GFX/OpenGL/GLShaderProgram.hpp"
#include "GFX/OpenGL/GLTexture.hpp"
#include "Geometry/Mesh.hpp"
#include "Material.hpp"
#include "PointLight.hpp"

namespace gk::rendering {

class RenderObject {
 public:
  RenderObject(const gk::geometry::Mesh& mesh,
               const std::shared_ptr<gfx::gl::GLShaderProgram> program,
               const std::vector<gfx::gl::GLTexture>& textures,
               const std::shared_ptr<Material> material);
  RenderObject(const RenderObject&) = delete;
  RenderObject& operator=(const RenderObject&) = delete;
  void bind() const;
  void draw(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix,
            const glm::mat4& modelMatrix,
            const std::vector<std::pair<PointLight, glm::vec3>>& lights) const;
  void update(const gk::geometry::Mesh& mesh);

  gfx::gl::GLMesh& mesh() noexcept;
  gfx::gl::GLShaderProgram& program() noexcept;
  Material& material() noexcept;
  std::vector<gfx::gl::GLTexture>& textures() noexcept;

  const gfx::gl::GLMesh& mesh() const noexcept;
  const gfx::gl::GLShaderProgram& program() const noexcept;
  const Material& material() const noexcept;
  const std::span<const gfx::gl::GLTexture> textures() const noexcept;

 private:
  std::unique_ptr<gfx::gl::GLMesh> m_mesh;
  std::shared_ptr<gfx::gl::GLShaderProgram> m_program;
  std::shared_ptr<Material> m_material;
  std::vector<gfx::gl::GLTexture> m_textures;
};

}  // namespace gk::rendering
