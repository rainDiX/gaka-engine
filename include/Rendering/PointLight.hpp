/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <glm/glm.hpp>

struct PointLight {
  glm::vec3 color;
  // intensity between 0 and 1
  float intensity;
  // Maximum range of the light, 0 is no limit
  float range;
  // amount the light dims along the distance of the light
  float decay;
};
