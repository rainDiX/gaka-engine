/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstddef>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

namespace gk::geometry {

class Curve {
 public:
  virtual ~Curve() {}
  virtual const std::vector<unsigned>& indices() = 0;
  virtual const std::vector<glm::vec3>& curve() = 0;
};

class SimpleCurve : Curve {
 public:
  void registerPoint(glm::vec2 point);
  void registerPoint(glm::vec3 point);
  void registerPoints(std::vector<glm::vec2> points);
  void registerPoints(std::vector<glm::vec3> points);
  const std::vector<unsigned>& indices() override;
  const std::vector<glm::vec3>& curve() override;
  glm::vec3& operator[](size_t index);

 private:
  void updateIndices();
  std::vector<glm::vec3> m_points;
  std::vector<unsigned> m_indices;
};

template <size_t N>
class Bezier : Curve {
 public:
  Bezier(const glm::vec3& begin, const glm::vec3& end, unsigned int nbSegments = 10);

  const std::vector<unsigned>& indices() override;
  const std::vector<glm::vec3>& curve() override;

  const std::vector<glm::vec3>& ctrlPoints();
  const std::unique_ptr<SimpleCurve> ctrlCurve();

  unsigned int nbSegments();
  void setNbSegments(unsigned int nbSegments);

  glm::vec3& operator[](size_t index);

 private:
  void evaluate();
  void updateIndices();
  glm::vec3 m_ctrlPoints[N];
  std::vector<glm::vec3> m_curvePoints;
  std::vector<unsigned> m_indices;
  unsigned int m_nbPoints;
};

// TODO: PieceWiseBezier

}  // namespace gk::geometry

#include "Bezier.tpp"
