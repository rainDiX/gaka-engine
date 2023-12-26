/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "../Geometry/Mesh.hpp"
#include "Material.hpp"
#include "PointLight.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"

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

class RenderObject {
   public:
    RenderObject(const gk::geometry::Mesh& mesh, const std::shared_ptr<ShaderProgram> program,
                 const std::vector<Texture>& textures,
                 const std::shared_ptr<Material> material);
    RenderObject(const RenderObject&) = delete;
    RenderObject& operator=(const RenderObject&) = delete;
    ~RenderObject();
    void bind() const;
    void draw(
        const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix,
        const glm::mat4& modelMatrix,
        const std::vector<std::pair<PointLight, glm::vec3>>& lights) const;
    void update(const gk::geometry::Mesh& mesh);
    void setDrawingMode(const DrawingMode mode);
    DrawingMode drawingMode() const;

   private:
    GLuint m_vao, m_vbo, m_ebo;
    GLint m_indexCount;
    size_t m_vertexBufferSize;
    DrawingMode m_drawingMode;
    std::shared_ptr<ShaderProgram> m_program;
    std::shared_ptr<Material> m_material;
    std::vector<Texture> m_textures;
};
