/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace gk::animation {

struct Bone {
  glm::mat4 tr;
  glm::vec3 joint;
  int numChild;
  int parent;
  std::array<int, 4> children;
};

class Skeleton {
 public:
  Skeleton(const glm::mat4& tr, glm::vec3 joint);
  const std::vector<Bone>& bones() const;

  int addBone(const glm::mat4& tr, glm::vec3 joint, int parent);
  void moveBone(int index, float angle, const glm::vec3& axis);

  Bone& operator[](int index);

  int size() const;

 private:
  void propagateTransform(const glm::mat4& tr, Bone& bone);
  std::vector<Bone> m_bones;
};

}  // namespace gk::animation
