/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <epoxy/gl.h>

#include <cstddef>
#include <span>

namespace gk::gfx::gl {
enum GLTextureType {
  DIFFUSE,
  SPECULAR,
};

class GLTexture {
 public:
  GLTexture(const std::span<const std::byte> data, GLsizei width, GLsizei height);
  GLTexture() = delete;
  ~GLTexture();
  GLTexture(const GLTexture&) = delete;
  GLTexture& operator=(const GLTexture&) = delete;
  void bind() const noexcept;

 private:
  GLuint m_id;
  GLTextureType txType;
};

}  // namespace gk::gfx::gl
