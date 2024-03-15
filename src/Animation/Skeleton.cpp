/*
 * SPDX-License-Identifier: MIT
 */

#include "Animation/Skeleton.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>

namespace gk::animation {
Skeleton::Skeleton(const glm::mat4& tr, glm::vec3 joint) {
  m_bones.push_back({tr, joint, 0, -1, {}});
}

int Skeleton::addBone(const glm::mat4& tr, glm::vec3 joint, int parent) {
  if (m_bones[parent].numChild >= 4) {
    return -1;
  }
  m_bones.push_back({tr, joint, 0, parent, {}});
  m_bones[parent].children[m_bones[parent].numChild++] = m_bones.size() - 1;
  return m_bones.size() - 1;
}

Bone& Skeleton::operator[](int index) { return m_bones[index]; }

int Skeleton::size() const { return m_bones.size(); }

void Skeleton::propagateTransform(const glm::mat4& tr, Bone& bone) {
  bone.joint = glm::vec3(tr * glm::vec4(bone.joint, 1.0f));
  bone.tr = tr * bone.tr;
  for (int i = 0; i < bone.numChild; i++) {
    propagateTransform(tr, m_bones[bone.children[i]]);
  }
}

void Skeleton::moveBone(int index, float angle, const glm::vec3& axis) {
  Bone& bone = m_bones[index];
  if (bone.parent == -1) return;

  glm::vec3 center = m_bones[bone.parent].joint;

  glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), -center);
  glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
  glm::mat4 translationBack = glm::translate(glm::mat4(1.0f), center);

  propagateTransform(translationBack * rotation * translationToOrigin, bone);
}

}  // namespace gk::animation
