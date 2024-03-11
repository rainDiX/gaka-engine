/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <glm/glm.hpp>

namespace gk::gfx {

struct PointLight {
  glm::vec3 color{1.0};
  // intensity between 0 and 1
  float intensity = 1.0;
  // Maximum range of the light, 0 is no limit
  float range = 0;
  // amount the light dims along the distance of the light
  float decay = 0;
};

}  // namespace gk::gfx
