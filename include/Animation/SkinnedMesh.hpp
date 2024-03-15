/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace gk::animation {


struct SkinnedMesh {

  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    int boneCount;
    glm::ivec4 boneIdx;
    glm::vec4 boneWeights;
  };

  std::vector<Vertex> vertices;
  std::vector<unsigned> indices;
};
}  // namespace gk::animation
