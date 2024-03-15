/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "GFX/OpenGL/GLShaderProgram.hpp"
#include "IO/RessourceManager.hpp"

namespace gk::gfx {

class Material {
 public:
  Material(std::unique_ptr<gl::ShaderProgram>&& program);
  Material(const Material&) = delete;
  Material(Material&&) = default;
  Material& operator=(const Material&) = delete;
  Material& operator=(Material&&) = default;

  gl::ShaderProgram& program() const noexcept;

 private:
  std::unique_ptr<gl::ShaderProgram> m_program;
};

inline Material createMaterial(io::RessourceManager& ressourceManager,
                                                const std::string& vertexShader,
                                                const std::string& fragmentShader);
Material createNormalMaterial(io::RessourceManager&);
Material createParametricMaterial(io::RessourceManager&);
Material createPhongMaterial(io::RessourceManager&);
Material createPhongMaterialAnimated(io::RessourceManager&);

}  // namespace gk::gfx
