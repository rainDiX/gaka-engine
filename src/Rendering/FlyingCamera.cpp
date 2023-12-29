#include "Rendering/FlyingCamera.hpp"

FlyingCamera::FlyingCamera(glm::vec3&& position, glm::vec3&& viewPoint, glm::vec3&& up)
    : m_position{position}, m_viewPoint{viewPoint}, m_up{up}, m_fov{45.0f} {}

float FlyingCamera::fov() const { return m_fov; }

void FlyingCamera::setFov(float fov) { m_fov = fov; }

void FlyingCamera::moveBy(float distance) {
  auto offset = getNormalizedViewVector() * distance;
  m_position += offset;
  m_viewPoint += offset;
}

void FlyingCamera::moveUp(float distance) {
  auto offset = m_up * distance;
  m_position += offset;
  m_viewPoint += offset;
}

void FlyingCamera::strafeBy(float distance) {
  auto strafeVector = glm::normalize(glm::cross(getNormalizedViewVector(), m_up)) * distance;

  m_position += strafeVector;
  m_viewPoint += strafeVector;
}

glm::vec3 FlyingCamera::getNormalizedViewVector() const {
  return glm::normalize(m_viewPoint - m_position);
}

void FlyingCamera::rotateBy(float angle, glm::vec3&& axis) {
  auto rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
  auto rotatedViewVector = rotationMatrix * glm::vec4(getNormalizedViewVector(), 0.0f);
  m_viewPoint = m_position + glm::vec3(rotatedViewVector);
}

void FlyingCamera::rotateRight(float angle) { rotateBy(angle, glm::vec3(0.0f, 1.0f, 0.0f)); }

void FlyingCamera::rotateUp(float angle) { rotateBy(angle, glm::vec3(0.0f, 0.0f, 1.0f)); }

glm::mat4 FlyingCamera::getViewMatrix() const { return glm::lookAt(m_position, m_viewPoint, m_up); }
