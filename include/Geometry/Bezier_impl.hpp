/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <iterator>
#include <stdexcept>
#include <vector>

#include "Geometry/algorithm.hpp"
#include "Geometry/Curves.hpp"

template <size_t N>
Bezier<N>::Bezier(const glm::vec3& begin, const glm::vec3& end,
                  unsigned int nbSegments) {
    for (int i = 0; i < N; ++i) {
        float d = float(i) / float(N - 1);
        m_ctrlPoints[i] = begin * (1.0f - d) + end * d;
    }
    m_nbPoints = 2 * nbSegments;
    evaluate();
}

template <size_t N>
const std::vector<unsigned>& Bezier<N>::indices() {
    return m_indices;
}

template <size_t N>
const std::vector<glm::vec3>& Bezier<N>::curve() {
    return m_curvePoints;
}

template <size_t N>
unsigned int Bezier<N>::nbSegments() {
    return 2 * m_nbPoints;
}

template <size_t N>
void Bezier<N>::setNbSegments(unsigned int nbSegments) {
    m_nbPoints = nbSegments * 2;
}

template <size_t N>
const std::unique_ptr<SimpleCurve> Bezier<N>::ctrlCurve() {
    auto curve = std::make_unique<SimpleCurve>();
    curve->registerPoints(std::vector<glm::vec3>(std::begin(m_ctrlPoints), std::end(m_ctrlPoints)));
    return curve;
}

template <size_t N>
const std::vector<glm::vec3>& Bezier<N>::ctrlPoints() {
    return m_curvePoints;
}

template <size_t N>
glm::vec3& Bezier<N>::operator[](size_t index) {
    if (index < 0 || index >= N) {
        throw std::out_of_range("Index out of range");
    }
    return m_ctrlPoints[index];
}

template <size_t N>
void Bezier<N>::evaluate() {
    m_curvePoints = std::vector<glm::vec3>();
    m_curvePoints.reserve(m_nbPoints);
    float epsilon = 1.0 / float(m_nbPoints);
    for (float u = 0.0; u < 1.0f; u += epsilon) {
        m_curvePoints.push_back(
            deCasteljau(u, std::begin(m_ctrlPoints), std::end(m_ctrlPoints)));
    }
    updateIndices();
}

template <size_t N>
void Bezier<N>::updateIndices() {
    m_indices = std::vector<unsigned int>();
    m_indices.reserve(m_curvePoints.size() * 2);
    for (int i = 0; i < m_curvePoints.size() - 1; ++i) {
        m_indices.push_back(i);
        m_indices.push_back(i+1);
    }
    m_indices.push_back(m_curvePoints.size() - 1);
}
