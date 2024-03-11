/*
 * SPDX-License-Identifier: MIT
 */

#include "GFX/Material.hpp"

#include <memory>
#include <string>

#include "GFX/Material.hpp"
#include "IO/RessourceManager.hpp"

namespace gk::gfx {

Material::Material(std::unique_ptr<gl::ShaderProgram>&& program) : m_program(std::move(program)) {}

gl::ShaderProgram& Material::program() const noexcept { return *m_program; }

inline Material createMaterial(io::RessourceManager& ressourceManager,
                               const std::string& vertexShader, const std::string& fragmentShader) {
  auto program = std::make_unique<gfx::gl::ShaderProgram>();
  program->compileFile(vertexShader, ressourceManager, gfx::gl::ShaderType::VERTEX);
  program->compileFile(fragmentShader, ressourceManager, gfx::gl::ShaderType::FRAGMENT);
  program->link();
  return Material(std::move(program));
}

Material createNormalMaterial(io::RessourceManager& ressourceManager) {
  return createMaterial(ressourceManager, "shaders/OpenGL/mesh.vert",
                        "shaders/OpenGL/normals.frag");
}

Material createParametricMaterial(io::RessourceManager& ressourceManager) {
  return createMaterial(ressourceManager, "shaders/OpenGL/mesh.vert",
                        "shaders/OpenGL/parametric.frag");
}

Material createLambertianMaterial(io::RessourceManager& ressourceManager) {
  return createMaterial(ressourceManager, "shaders/OpenGL/mesh.vert",
                        "shaders/OpenGL/lambertian.frag");
}

Material createPhongMaterial(io::RessourceManager& ressourceManager) {
  return createMaterial(ressourceManager, "shaders/OpenGL/mesh.vert", "shaders/OpenGL/phong.frag");
}

}  // namespace gk::gfx
