#include "FlyingCamera.hpp"

FlyingCamera::FlyingCamera(glm::vec3&& position, glm::vec3&& viewPoint, glm::vec3&& up)
    : position_{position}, viewPoint_{viewPoint}, up_{up}, fov_{45.0f} {}

float FlyingCamera::fov() const {
    return fov_;
}

void FlyingCamera::setFov(float fov) {
    fov_ = fov;
}

void FlyingCamera::moveBy(float distance) {
    auto offset = getNormalizedViewVector() * distance;
    position_ += offset;
    viewPoint_ += offset;
}

void FlyingCamera::moveUp(float distance) {
    auto offset = up_ * distance;
    position_ += offset;
    viewPoint_ += offset;
}

void FlyingCamera::strafeBy(float distance) {
    auto strafeVector =
        glm::normalize(glm::cross(getNormalizedViewVector(), up_)) * distance;

    position_ += strafeVector;
    viewPoint_ += strafeVector;
}

glm::vec3 FlyingCamera::getNormalizedViewVector() const {
    return glm::normalize(viewPoint_ - position_);
}

void FlyingCamera::rotateBy(float angle, glm::vec3&& axis) {
    auto rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    auto rotatedViewVector = rotationMatrix * glm::vec4(getNormalizedViewVector(), 0.0f);
    viewPoint_ = position_ + glm::vec3(rotatedViewVector);
}

void FlyingCamera::rotateRight(float angle) {
    rotateBy(angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void FlyingCamera::rotateUp(float angle) {
    rotateBy(angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::mat4 FlyingCamera::getViewMatrix() const {
    return glm::lookAt(position_, viewPoint_, up_);
}

