/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <GL/glew.h>
#include <sys/types.h>

#include <cstddef>
#include <glm/glm.hpp>
#include <span>

#include "GFX/OpenGL/GLHelperFn.hpp"
#include "GFX/OpenGL/GLShaderProgram.hpp"

namespace gk::gfx::gl {

enum DrawingMode {
  POINTS = GL_POINTS,
  LINES = GL_LINES,
  LINE_LOOP = GL_LINE_LOOP,
  LINE_STRIP = GL_LINE_STRIP,
  TRIANGLES = GL_TRIANGLES,
  TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
  TRIANGLE_FAN = GL_TRIANGLE_FAN,
  LINES_ADJACENCY = GL_LINES_ADJACENCY,
  LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
  TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
  TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
};

enum BufferType { ARRAY = GL_ARRAY_BUFFER, ELEMENT = GL_ELEMENT_ARRAY_BUFFER };

class GLMesh {
 public:
  template <typename V>
  GLMesh(const std::span<const V>& vertices, const GLShaderProgram* program,
         DrawingMode drawingMode = TRIANGLES);

  template <typename V>
  GLMesh(const std::span<const V>& vertices, const std::span<const uint>& indices,
         const GLShaderProgram* program, DrawingMode drawingMode = TRIANGLES);
  GLMesh() = delete;
  GLMesh(const GLMesh&) = delete;
  GLMesh& operator=(const GLMesh&) = delete;
  ~GLMesh();
  void bind() const noexcept;
  void draw() const noexcept;

  template <typename V>
  void update(const std::span<const V>& vertices) noexcept;
  template <typename V>
  void update(const std::span<const V>& vertices, const std::span<const uint>& indices) noexcept;

  void setDrawingMode(const DrawingMode mode) noexcept;
  DrawingMode drawingMode() const noexcept;
  void setBufferType(const BufferType buftype) noexcept;
  BufferType bufferType() const noexcept;

 private:
  GLuint m_vao, m_vbo, m_ebo;
  GLint m_indexBufferSize;
  size_t m_vertexBufferSize;
  DrawingMode m_drawingMode;
  BufferType m_bufferType;
};

template <typename V>
GLMesh::GLMesh(const std::span<const V>& vertices, const GLShaderProgram* program,
               DrawingMode drawingMode)
    : m_drawingMode(drawingMode) {
  m_bufferType = ARRAY;
  m_vao = 0;
  m_vbo = 0;
  m_ebo = 0;
  setupVertexObjects(m_vao, m_vbo, vertices);
  program->setupAttribPointer();
  m_vertexBufferSize = vertices.size();
}

template <typename V>
GLMesh::GLMesh(const std::span<const V>& vertices, const std::span<const uint>& indices,
               const GLShaderProgram* program, DrawingMode drawingMode)
    : m_drawingMode(drawingMode) {
  m_bufferType = ELEMENT;
  m_vao = 0;
  m_vbo = 0;
  m_ebo = 0;
  setupVertexObjects(m_vao, m_vbo, vertices);
  setupElementObjects(m_ebo, indices);
  program->setupAttribPointer();

  m_indexBufferSize = indices.size();
  m_vertexBufferSize = vertices.size();
}

template <typename V>
void GLMesh::update(const std::span<const V>& vertices) noexcept {
  m_vertexBufferSize = updateBuffer(m_vbo, vertices, m_vertexBufferSize, GL_ARRAY_BUFFER);
}

template <typename V>
void GLMesh::update(const std::span<const V>& vertices,
                    const std::span<const uint>& indices) noexcept {
  m_vertexBufferSize = updateBuffer(m_vbo, vertices, m_vertexBufferSize, GL_ARRAY_BUFFER);
  m_indexBufferSize = updateBuffer(m_ebo, indices, m_indexBufferSize, GL_ELEMENT_ARRAY_BUFFER);
}

}  // namespace gk::gfx::gl
