/*
 * SPDX-License-Identifier: MIT
 */
#include "GFX/OpenGL/GLMesh.hpp"

#include <GFX/OpenGL/GLHelperFn.hpp>

namespace gk::gfx::gl {

void Mesh::bind() const noexcept {
  if (m_vao > 0) {
    glBindVertexArray(m_vao);
  }
}

void Mesh::draw() const noexcept {
  if (m_bufferType == ELEMENT) {
    glDrawElements(m_drawingMode, m_indexBufferSize, GL_UNSIGNED_INT, nullptr);
  } else {
    glDrawArrays(m_drawingMode, 0, m_vertexBufferSize);
  }
}

void Mesh::setDrawingMode(const DrawingMode mode) noexcept { m_drawingMode = mode; }

DrawingMode Mesh::drawingMode() const noexcept { return m_drawingMode; }

void Mesh::setBufferType(const BufferType buftype) noexcept {
  if (buftype != m_bufferType) {
    m_bufferType = buftype;
    switch (m_bufferType) {
      case ARRAY:
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
        break;
      case ELEMENT:
        glGenBuffers(1, &m_ebo);
        break;
    }
  }
}

BufferType Mesh::bufferType() const noexcept { return m_bufferType; }

Mesh::~Mesh() {
  if (m_vbo > 0) {
    glDeleteBuffers(1, &m_vbo);
  }
  if (m_ebo > 0) {
    glDeleteBuffers(1, &m_ebo);
  }
  if (m_vao > 0) {
    glDeleteVertexArrays(1, &m_vao);
  }
}

}  // namespace gk::gfx::gl
