/*
 * SPDX-License-Identifier: MIT
 */

#include "Geometry/Curves.hpp"

#include <cstddef>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <stdexcept>
#include <vector>

void gk::geometry::SimpleCurve::registerPoint(glm::vec2 point) {
    m_points.push_back(glm::vec3(point, 0.0));
    updateIndices();
}

void gk::geometry::SimpleCurve::registerPoint(glm::vec3 point) {
    m_points.push_back(point);
    updateIndices();
}

void gk::geometry::SimpleCurve::registerPoints(std::vector<glm::vec2> points) {
    for (auto point : points) {
        m_points.push_back(glm::vec3(point, 0.0));
    }
    updateIndices();
}
void gk::geometry::SimpleCurve::registerPoints(std::vector<glm::vec3> points) {
    for (auto point : points) {
        m_points.push_back(point);
    }
    updateIndices();
}

inline void gk::geometry::SimpleCurve::updateIndices() {
    m_indices.reserve(m_points.size() * 2);
    for (size_t i = 0; i < m_points.size(); ++i) {
        m_indices.push_back(i);
        if (i < m_points.size()) {
            m_indices.push_back(i + 1);
        }
    }
}

glm::vec3& gk::geometry::SimpleCurve::operator[](size_t index) {
    if (index < 0 || index >= m_points.size()) {
        throw std::out_of_range("Index out of range");
    }
    return m_points[index];
}


const std::vector<unsigned>& gk::geometry::SimpleCurve::indices() { return m_indices; }

const std::vector<glm::vec3>& gk::geometry::SimpleCurve::curve() { return m_points; }

// TODO: PieceWiseBezier
