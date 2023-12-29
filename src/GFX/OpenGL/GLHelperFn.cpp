/*
 * SPDX-License-Identifier: MIT
 */
#include <GFX/OpenGL/GLHelperFn.hpp>

namespace gk::gfx::gl {
void setupElementObjects(GLuint& ebo, const std::span<const GLuint>& indices) {
  glGenBuffers(1, &ebo);
  updateBuffer(ebo, indices, 0, GL_ELEMENT_ARRAY_BUFFER);
}
}  // namespace gk::gfx::gl
