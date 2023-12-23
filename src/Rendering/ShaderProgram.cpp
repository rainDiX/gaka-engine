/*
 * SPDX-License-Identifier: MIT
 */

#include "Rendering/ShaderProgram.hpp"

#include <iostream>
#include <vector>

ShaderProgram::ShaderProgram() { m_id = glCreateProgram(); }

ShaderProgram::~ShaderProgram() { glDeleteProgram(m_id); }

GLint ShaderProgram::id() const { return m_id; }

bool ShaderProgram::isLinked() const { return m_linked; }

void ShaderProgram::use() const {
    if (m_linked) {
        glUseProgram(m_id);
    }
}

GLint ShaderProgram::getUniformLocation(const std::string& name) const {
    return glGetUniformLocation(m_id, name.c_str());
}

GLint ShaderProgram::getAttributeLocation(const std::string& name) const {
    return glGetAttribLocation(m_id, name.c_str());
}

const std::vector<VertexAttribute>& ShaderProgram::getAttributes() const {
    return m_attributes;
}

void ShaderProgram::compileSource(const std::string& source,
                                  ShaderType type) const {
    auto shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    auto success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == 0) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    } else {
        glAttachShader(m_id, shader);
    }
}
void ShaderProgram::compileFile(const std::string& relativePath,
                                AssetManager& assetManager,
                                ShaderType type) const {
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

void ShaderProgram::updateAttributes() {
    GLint active_attrs = 0;
    // Requires OpenGL 4.3+
    glGetProgramInterfaceiv(m_id, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES,
                            &active_attrs);

    std::vector<GLenum> properties = {GL_NAME_LENGTH, GL_TYPE};
    std::vector<GLint> values(properties.size(), 0);
    std::vector<GLchar> name_bytes;
    std::size_t offset = 0;

    m_attributes = std::vector<VertexAttribute>();
    m_attributes.reserve(active_attrs);

    for (GLuint attrib = 0; attrib < active_attrs; ++attrib) {
        glGetProgramResourceiv(m_id, GL_PROGRAM_INPUT, attrib,
                               properties.size(), properties.data(),
                               values.size(), nullptr, values.data());

        name_bytes.resize(values[0], 0);

        glGetProgramResourceName(m_id, GL_PROGRAM_INPUT, attrib,
                                 name_bytes.size(), nullptr, name_bytes.data());
        const auto [num, gl_type, size] = componentsTypeSize(values[1]);
        const std::string name(name_bytes.data(), values[0] - 1);
        m_attributes.push_back(VertexAttribute{
            .name = name,
            .size = num,
            .type_enum = gl_type,
            .stride = 0,
            .offset = (void*)offset,
        });
        offset += size;
    }
    for (auto& attrib : m_attributes) {
        attrib.stride = offset;
    }
}

void ShaderProgram::link() {
    if (!m_linked) {
        glLinkProgram(m_id);
        auto success = 0;
        char infoLog[512];
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(m_id, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
            deleteAttachedShaders(m_id);
        } else {
            m_linked = true;
            updateAttributes();
            deleteAttachedShaders(m_id);
        }
    }
}

template <>
void ShaderProgram::setUniform<bool>(const std::string& name,
                                     bool value) const {
    glUniform1i(getUniformLocation(name), GLint(value));
}

template <>
void ShaderProgram::setUniform<GLint>(const std::string& name,
                                      GLint value) const {
    glUniform1i(getUniformLocation(name), value);
}

template <>
void ShaderProgram::setUniform<GLfloat>(const std::string& name,
                                        GLfloat value) const {
    glUniform1f(getUniformLocation(name), value);
}

template <>
void ShaderProgram::setUniform<const glm::vec2*>(const std::string& name,
                                                 const glm::vec2* value) const {
    glUniform2f(getUniformLocation(name), value->x, value->y);
}

template <>
void ShaderProgram::setUniform<const glm::vec3*>(const std::string& name,
                                                 const glm::vec3* value) const {
    glUniform3f(getUniformLocation(name), value->x, value->y, value->z);
}

template <>
void ShaderProgram::setUniform<const glm::vec4*>(const std::string& name,
                                                 const glm::vec4* value) const {
    glUniform4f(getUniformLocation(name), value->x, value->y, value->z,
                value->w);
}

template <>
void ShaderProgram::setUniform<const glm::mat3*>(const std::string& name,
                                                 const glm::mat3* value) const {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE,
                       (const float*)value);
}

template <>
void ShaderProgram::setUniform<const glm::mat4*>(const std::string& name,
                                                 const glm::mat4* value) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
                       (const float*)value);
}
