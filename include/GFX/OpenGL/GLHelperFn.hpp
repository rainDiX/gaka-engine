/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <epoxy/gl.h>

#include <span>
#include <tuple>

namespace gk::gfx::gl {
inline std::tuple<int, GLenum, size_t> componentsTypeSize(GLenum type_enum) {
  switch (type_enum) {
    case GL_FLOAT:
      return std::make_tuple(1, GL_FLOAT, sizeof(GLfloat));
    case GL_FLOAT_VEC2:
      return std::make_tuple(2, GL_FLOAT, 2 * sizeof(GLfloat));
    case GL_FLOAT_VEC3:
      return std::make_tuple(3, GL_FLOAT, 3 * sizeof(GLfloat));
    case GL_FLOAT_VEC4:
      return std::make_tuple(4, GL_FLOAT, 4 * sizeof(GLfloat));
    case GL_INT:
      return std::make_tuple(1, GL_INT, sizeof(GLint));
    case GL_INT_VEC2:
      return std::make_tuple(2, GL_INT, 2 * sizeof(GLint));
    case GL_INT_VEC3:
      return std::make_tuple(3, GL_INT, 3 * sizeof(GLint));
    case GL_INT_VEC4:
      return std::make_tuple(4, GL_INT, 4 * sizeof(GLint));
    case GL_UNSIGNED_INT:
      return std::make_tuple(1, GL_UNSIGNED_INT, sizeof(GLuint));
    case GL_UNSIGNED_INT_VEC2:
      return std::make_tuple(2, GL_UNSIGNED_INT, 2 * sizeof(GLuint));
    case GL_UNSIGNED_INT_VEC3:
      return std::make_tuple(3, GL_UNSIGNED_INT, 3 * sizeof(GLuint));
    case GL_UNSIGNED_INT_VEC4:
      return std::make_tuple(4, GL_UNSIGNED_INT, 4 * sizeof(GLuint));
    case GL_DOUBLE:
      return std::make_tuple(1, GL_DOUBLE, sizeof(GLdouble));
    case GL_BOOL:
      return std::make_tuple(1, GL_BOOL, sizeof(GLboolean));
    default:
      return std::make_tuple(0, 0, 0);
  }
};

template <typename T>
inline size_t updateBuffer(GLuint handle, const std::span<const T>& buffer, size_t old_buf_size,
                           GLenum target) {
  glBindBuffer(target, handle);
  if (old_buf_size != buffer.size()) {
    glBufferData(target, buffer.size() * sizeof(T), buffer.data(), GL_STATIC_DRAW);
  } else {
    glBufferSubData(target, 0, buffer.size() * sizeof(T), buffer.data());
  }
  return buffer.size();
}

template <typename T>
void setupVertexObjects(GLuint& vao, GLuint& vbo, const std::span<const T>& v) {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  updateBuffer(vbo, v, 0, GL_ARRAY_BUFFER);
}

void setupElementObjects(GLuint& ebo, const std::span<const GLuint>& indices);

}  // namespace gk::gfx::gl
