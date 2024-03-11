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

class Texture {
 public:
  Texture(const std::span<const std::byte> data, GLsizei width, GLsizei height);
  Texture() = delete;
  ~Texture();
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;
  void bind() const noexcept;

 private:
  GLuint m_id;
  GLTextureType txType;
};

}  // namespace gk::gfx::gl
