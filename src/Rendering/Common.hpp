/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <tuple>
#include <glad/gl.h>

struct VertexAttribute {
    std::string name;
    GLint size;
    GLenum type_enum;
    GLsizei stride;
    GLvoid* offset;
};

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