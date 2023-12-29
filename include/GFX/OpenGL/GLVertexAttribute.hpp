/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <GL/glew.h>
#include <string>

namespace gk::gfx::gl {
struct GLVertexAttribute {
    std::string name;
    GLint size;
    GLenum type_enum;
    GLsizei stride;
    GLvoid* offset;
    GLint location;
};
}  // namespace gk::gk::gfx::gl
