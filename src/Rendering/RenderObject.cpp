#include "Rendering/RenderObject.hpp"

#include "Geometry/Mesh.hpp"
#include "Rendering/Common.hpp"

template <typename T>
size_t updateBuffer(GLuint handle, const std::vector<T>& buffer,
                    size_t old_buf_size, GLenum target) {
    glBindBuffer(target, handle);
    if (old_buf_size != buffer.size()) {
        glBufferData(target, buffer.size() * sizeof(T), buffer.data(),
                     GL_STATIC_DRAW);
    } else {
        glBufferSubData(target, 0, buffer.size() * sizeof(T), buffer.data());
    }
    return buffer.size();
}

template <typename T>
void setupVertexObjects(GLuint* vao, GLuint* vbo, const std::vector<T>& v) {
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);
    glGenBuffers(1, vbo);
    updateBuffer(*vbo, v, 0, GL_ARRAY_BUFFER);
}

void setupElementObjects(GLuint* ebo, const std::vector<GLuint>& indices) {
    glGenBuffers(1, ebo);
    updateBuffer(*ebo, indices, 0, GL_ELEMENT_ARRAY_BUFFER);
}

void setupAttribPointer(const std::vector<VertexAttribute>& attributes,
                        const ShaderProgram& program) {
    for (const auto& attrib : attributes) {
        GLint location = program.getAttributeLocation(attrib.name);
        glVertexAttribPointer(location, attrib.size, GL_FLOAT, GL_FALSE,
                              attrib.stride, attrib.offset);
        glEnableVertexAttribArray(location);
    }
}

RenderObject::RenderObject(const gk::geometry::Mesh& mesh,
                           const std::shared_ptr<ShaderProgram> program,
                           const std::vector<Texture>& textures,
                           const std::shared_ptr<Material> material)
    : m_program(program), m_material(material), m_textures(textures) {
    m_vao = 0;
    m_vbo = 0;
    m_ebo = 0;
    setupVertexObjects(&m_vao, &m_vbo, mesh.vertices);
    setupAttribPointer(m_program->getAttributes(), *m_program);
    setupElementObjects(&m_ebo, mesh.indices);

    m_indexCount = mesh.indices.size();
    m_vertexBufferSize = mesh.vertices.size();
    m_drawingMode = DrawingMode::TRIANGLES;
}

void RenderObject::bind() const {
    if (m_vao > 0) {
        glBindVertexArray(m_vao);
    }
}

void RenderObject::draw(
    const glm::mat4& projection_matrix, const glm::mat4& view_matrix,
    const glm::mat4& model_matrix,
    const std::vector<std::pair<PointLight, glm::vec3>>& lights) const {
    bind();
    m_program->use();
    m_program->setUniform("projection", &projection_matrix);
    m_program->setUniform("view", &view_matrix);
    m_program->setUniform("model", &model_matrix);

    m_program->setUniform<const glm::vec3*>("material.ambient",
                                            &(m_material->ambient));
    m_program->setUniform<const glm::vec3*>("material.diffuse",
                                            &m_material->diffuse);
    m_program->setUniform<const glm::vec3*>("material.specular",
                                            &m_material->specular);
    m_program->setUniform("material.shininess", m_material->shininess);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_program->setUniform("nb_point_lights", static_cast<GLint>(lights.size()));
    for (int i = 0; i < int(lights.size()); i++) {
        m_program->setUniform("pointLights[" + std::to_string(i) + "].color",
                              &lights[i].first.color);
        m_program->setUniform(
            "pointLights[" + std::to_string(i) + "].intensity",
            lights[i].first.intensity);
        m_program->setUniform("pointLights[" + std::to_string(i) + "].range",
                              lights[i].first.range);
        m_program->setUniform("pointLights[" + std::to_string(i) + "].decay",
                              lights[i].first.decay);
        m_program->setUniform("pointLights[" + std::to_string(i) + "].position",
                              &lights[i].second);
    }
    glDrawElements(m_drawingMode, m_indexCount, GL_UNSIGNED_INT, nullptr);
}

void RenderObject::update(const gk::geometry::Mesh& mesh) {
    m_indexCount = static_cast<GLint>(mesh.indices.size());
    m_vertexBufferSize =
        updateBuffer(m_vbo, mesh.vertices, m_vertexBufferSize, GL_ARRAY_BUFFER);
    m_vertexBufferSize = updateBuffer(m_ebo, mesh.indices, m_vertexBufferSize,
                                      GL_ELEMENT_ARRAY_BUFFER);
}

void RenderObject::setDrawingMode(const DrawingMode mode) {
    m_drawingMode = mode;
}

DrawingMode RenderObject::drawingMode() const { return m_drawingMode; }

RenderObject::~RenderObject() {
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
