/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <span>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_enums.hpp>

#include "IO/RessourceManager.hpp"

namespace gk::gfx::vulkan {

class GLShaderProgram {
 public:
  GLShaderProgram();
  ~GLShaderProgram();
  GLShaderProgram(const GLShaderProgram&) = delete;
  GLShaderProgram& operator=(const GLShaderProgram&) = delete;
  bool isLinked() const noexcept;
  void use() const noexcept;
  void compileSource(const std::string& source, vk::ShaderStageFlagBits type) const noexcept;
  void compileFile(const std::string& relativePath, io::RessourceManager& assetManager,
                   ShaderType type) const noexcept;
  void link() noexcept;
  void enableVertexAttributes() const noexcept;

  template <typename T>
  void setUniform(const std::string& name, const T value) const noexcept;

 private:
  void updateAttributes() noexcept;
  bool m_linked = false;
};

}  // namespace gk::gfx::gl
