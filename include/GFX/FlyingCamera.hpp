/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gk::gfx {
class FlyingCamera {
 public:
  FlyingCamera(glm::vec3&& position, glm::vec3&& viewPoint, glm::vec3&& up);
  float fov() const;
  void setFov(float fov);
  void moveBy(float distance);
  void moveUp(float distance);
  void strafeBy(float distance);
  void rotateRight(float angle);
  void rotateUp(float angle);
  glm::mat4 getViewMatrix() const;

 private:
  glm::vec3 m_position{0.0};
  glm::vec3 m_viewPoint{0.0};
  glm::vec3 m_up{0.0};
  float m_fov = 0;
  glm::vec3 getNormalizedViewVector() const;
  void rotateBy(float angle, glm::vec3&& axis);
};
}  // namespace gk::gfx
