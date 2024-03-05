/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <epoxy/gl.h>

#include <span>
#include <string>
#include <vector>

#include "GFX/OpenGL/GLVertexAttribute.hpp"
#include "IO/RessourceManager.hpp"

namespace gk::gfx::gl {

enum ShaderType {
  VERTEX = GL_VERTEX_SHADER,
  FRAGMENT = GL_FRAGMENT_SHADER,
  GEOMETRY = GL_GEOMETRY_SHADER,
  COMPUTE = GL_COMPUTE_SHADER,
  TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
  TESS_CONTROL = GL_TESS_CONTROL_SHADER,
};

class GLShaderProgram {
 public:
  GLShaderProgram();
  ~GLShaderProgram();
  GLShaderProgram(const GLShaderProgram&) = delete;
  GLShaderProgram& operator=(const GLShaderProgram&) = delete;
  GLint id() const noexcept;
  bool isLinked() const noexcept;
  void use() const noexcept;
  const std::span<const GLVertexAttribute> attributes() const noexcept;
  void compileSource(const std::string& source, ShaderType type) const noexcept;
  void compileFile(const std::string& relativePath, io::RessourceManager& assetManager,
                   ShaderType type) const noexcept;
  void link() noexcept;
  void enableVertexAttributes() const noexcept;

  template <typename T>
  void setUniform(const std::string& name, const T value) const noexcept;

 private:
  void updateAttributes() noexcept;
  GLuint m_id;
  bool m_linked = false;
  std::vector<GLVertexAttribute> m_attributes;
};

}  // namespace gk::gfx::gl
