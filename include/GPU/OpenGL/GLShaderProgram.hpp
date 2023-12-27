/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "IO/AssetManager.hpp"
#include "Rendering/Common.hpp"

namespace gk::gpu::gl {

enum ShaderType {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    Compute = GL_COMPUTE_SHADER,
    TessEvaluation = GL_TESS_EVALUATION_SHADER,
    TessControl = GL_TESS_CONTROL_SHADER,
};

class GLShaderProgram {
   public:
    GLShaderProgram();
    ~GLShaderProgram();
    GLShaderProgram(const GLShaderProgram&) = delete;
    GLShaderProgram& operator=(const GLShaderProgram&) = delete;
    GLint id() const;
    bool isLinked() const;
    void use() const;
    GLint getUniformLocation(const std::string& name) const;
    GLint getAttributeLocation(const std::string& name) const;
    const std::vector<VertexAttribute>& getAttributes() const;
    void compileSource(const std::string& source, ShaderType type) const;
    void compileFile(const std::string& relativePath, AssetManager& assetManager, ShaderType type) const;
    void link();

    template <typename T>
    void setUniform(const std::string& name, const T value) const;

   private:
    void updateAttributes();
    GLuint m_id;
    bool m_linked = false;
    std::vector<VertexAttribute> m_attributes;
};

}  // namespace gk::gpu::gl
