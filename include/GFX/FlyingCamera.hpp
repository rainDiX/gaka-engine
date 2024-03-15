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
  FlyingCamera(const glm::vec3& position, const glm::vec3& up, float fov = 45.f, float yaw = -90.f,
               float pitch = 0.f);

  float fov() const;
  void setFov(float fov);
  void moveBy(float distance);
  void strafeBy(float distance);
  void moveUp(float distance);
  void yawMotion(float angle);
  void pitchMotion(float angle);
  const glm::mat4& getViewMatrix();
  const glm::vec3& position() const;

 private:
  glm::vec3 m_position{0.0};
  glm::vec3 m_world_up{0.0};
  glm::vec3 m_up{0.0};
  glm::vec3 m_front{0.0};
  float m_fov;
  float m_yaw;
  float m_pitch = 0.f;
  glm::vec3 m_right{0.0};
  glm::mat4 m_viewMatrix;
  bool m_needUpdate = true;

  void updateCameraVectors();
};
}  // namespace gk::gfx
