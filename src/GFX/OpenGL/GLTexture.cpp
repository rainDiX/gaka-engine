/*
 * SPDX-License-Identifier: MIT
 */
#include "GFX/OpenGL/GLTexture.hpp"

#include <epoxy/gl_generated.h>

#include <cstddef>

namespace gk::gfx::gl {
GLTexture::GLTexture(const std::span<const std::byte> data, GLsizei width, GLsizei height) {
  glGenTextures(1, &m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLTexture::~GLTexture() { glDeleteTextures(1, &m_id); }

void GLTexture::bind() const noexcept {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_id);
}

}  // namespace gk::gfx::gl
