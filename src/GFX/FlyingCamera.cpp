#include "GFX/FlyingCamera.hpp"

namespace gk::gfx {
FlyingCamera::FlyingCamera(const glm::vec3& position, const glm::vec3& up, float fov, float yaw,
                           float pitch)
    : m_position{position}, m_world_up{up}, m_up{up}, m_fov{fov}, m_yaw(yaw), m_pitch(pitch) {
      updateCameraVectors();
    }

float FlyingCamera::fov() const { return m_fov; }

void FlyingCamera::setFov(float fov) { m_fov = fov; }

void FlyingCamera::moveBy(float distance) {
  auto offset = m_front * distance;
  m_position += offset;
  m_needUpdate = true;
}

void FlyingCamera::moveUp(float distance) {
  auto offset = m_up * distance;
  m_position += offset;
  m_needUpdate = true;
}

void FlyingCamera::strafeBy(float distance) {
  auto offset = m_right * distance;
  m_position += offset;
  m_needUpdate = true;
}

inline void updateAngle(float& origin, float angle) {
  origin += angle;
  if (origin > 180.f) origin -= 360.f;
  if (origin < -180.f) origin += 360.f;
}

void FlyingCamera::yawMotion(float angle) {
  updateAngle(m_yaw, angle);
  m_needUpdate = true;
}

void FlyingCamera::pitchMotion(float angle) {
  updateAngle(m_pitch, angle);
  m_needUpdate = true;
};

const glm::mat4& FlyingCamera::getViewMatrix() {
  if (m_needUpdate) {
    updateCameraVectors();
    m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
    m_needUpdate = false;
  }
  return m_viewMatrix;
}

const glm::vec3& FlyingCamera::position() const { return m_position; }

void FlyingCamera::updateCameraVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  front.y = sin(glm::radians(m_pitch));
  front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_front = glm::normalize(front);
  m_right = glm::normalize(glm::cross(m_front, m_world_up));
  m_up = glm::normalize(glm::cross(m_right, m_front));
}

}  // namespace gk::gfx
