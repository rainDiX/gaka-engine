/*
 * SPDX-License-Identifier: MIT
 */

#include "GFX/OpenGL/GLShaderProgram.hpp"

#include <iostream>
#include <span>
#include <vector>

#include "GFX/OpenGL/GLHelperFn.hpp"
#include "GFX/OpenGL/GLVertexAttribute.hpp"

namespace gk::gfx::gl {
GLShaderProgram::GLShaderProgram() { m_id = glCreateProgram(); }

GLShaderProgram::~GLShaderProgram() { glDeleteProgram(m_id); }

GLint GLShaderProgram::id() const noexcept { return m_id; }

bool GLShaderProgram::isLinked() const noexcept { return m_linked; }

void GLShaderProgram::use() const noexcept {
    if (m_linked) {
        glUseProgram(m_id);
    } else {
        std::cerr << "WARNING: Attempted to use an unlinked Program\n";
    }
}

const std::span<const GLVertexAttribute> GLShaderProgram::attributes() const noexcept { return m_attributes; }

void GLShaderProgram::compileSource(const std::string& source, ShaderType type) const noexcept {
    auto shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    auto success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == 0) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        glAttachShader(m_id, shader);
    }
}
void GLShaderProgram::compileFile(const std::string& relativePath, io::RessourceManager& assetManager,
                                  ShaderType type) const noexcept {
    auto source = assetManager.readString(relativePath);
    compileSource(*source, type);
}

void deleteAttachedShaders(GLint program) {
    GLint shaderCount = 0;
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &shaderCount);
    std::vector<GLuint> shaders(shaderCount);
    glGetAttachedShaders(program, shaderCount, NULL, shaders.data());

    for (auto shader : shaders) {
        glDetachShader(program, shader);
        glDeleteShader(shader);
    }
}

void GLShaderProgram::updateAttributes() noexcept {
    GLint active_attrs = 0;
    // Requires OpenGL 4.3+
    glGetProgramInterfaceiv(m_id, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &active_attrs);

    std::vector<GLenum> properties = {GL_NAME_LENGTH, GL_TYPE};
    std::vector<GLint> values(properties.size(), 0);
    std::vector<GLchar> name_bytes;
    std::size_t offset = 0;

    m_attributes.clear();
    m_attributes.reserve(active_attrs);

    for (GLint attrib = 0; attrib < active_attrs; ++attrib) {
        glGetProgramResourceiv(m_id, GL_PROGRAM_INPUT, attrib, properties.size(), properties.data(), values.size(),
                               nullptr, values.data());

        name_bytes.resize(values[0], 0);

        glGetProgramResourceName(m_id, GL_PROGRAM_INPUT, attrib, name_bytes.size(), nullptr, name_bytes.data());
        const auto [num, gl_type, size] = componentsTypeSize(values[1]);
        const std::string name(name_bytes.data(), values[0] - 1);
        GLint location =  glGetAttribLocation(m_id, name.c_str());
        m_attributes.push_back(GLVertexAttribute{
            .name = name,
            .size = num,
            .type_enum = gl_type,
            .stride = 0,
            .offset = (void*)offset,
            .location = location
        });
        offset += size;
    }
    for (auto& attrib : m_attributes) {
        attrib.stride = offset;
    }
}
void GLShaderProgram::setupAttribPointer() const noexcept {
    for (auto& attrib : m_attributes) {
        glVertexAttribPointer(attrib.location, attrib.size, GL_FLOAT, GL_FALSE, attrib.stride, attrib.offset);
        glEnableVertexAttribArray(attrib.location);
    }
}

void GLShaderProgram::link() noexcept {
    if (!m_linked) {
        glLinkProgram(m_id);
        auto success = 0;
        char infoLog[512];
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(m_id, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            deleteAttachedShaders(m_id);
        } else {
            m_linked = true;
            updateAttributes();
            setupAttribPointer();
            deleteAttachedShaders(m_id);
        }
    }
}

template <>
void GLShaderProgram::setUniform<bool>(const std::string& name, bool value) const noexcept {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), GLint(value));
}

template <>
void GLShaderProgram::setUniform<GLint>(const std::string& name, GLint value) const noexcept {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

template <>
void GLShaderProgram::setUniform<GLfloat>(const std::string& name, GLfloat value) const noexcept {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

template <>
void GLShaderProgram::setUniform<const glm::vec2*>(const std::string& name, const glm::vec2* value) const noexcept {
    glUniform2f(glGetUniformLocation(m_id, name.c_str()), value->x, value->y);
}

template <>
void GLShaderProgram::setUniform<const glm::vec3*>(const std::string& name, const glm::vec3* value) const noexcept {
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), value->x, value->y, value->z);
}

template <>
void GLShaderProgram::setUniform<const glm::vec4*>(const std::string& name, const glm::vec4* value) const noexcept {
    glUniform4f(glGetUniformLocation(m_id, name.c_str()), value->x, value->y, value->z, value->w);
}

template <>
void GLShaderProgram::setUniform<const glm::mat3*>(const std::string& name, const glm::mat3* value) const noexcept {
    glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, (const float*)value);
}

template <>
void GLShaderProgram::setUniform<const glm::mat4*>(const std::string& name, const glm::mat4* value) const noexcept {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, (const float*)value);
}

}  // namespace gk::gfx::gl
