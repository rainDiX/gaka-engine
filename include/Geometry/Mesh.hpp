/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace gk::geometry {
struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

struct Mesh {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};
}  // namespace gk::geometry
