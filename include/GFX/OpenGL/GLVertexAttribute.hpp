/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <epoxy/gl.h>

#include <string>

namespace gk::gfx::gl {
struct VertexAttribute {
  std::string name;
  GLint size;
  GLenum type_enum;
  GLsizei stride;
  GLvoid* offset;
  GLint location;
};
}  // namespace gk::gfx::gl
