/*
 * SPDX-License-Identifier: MIT
 */

#include "GFX/MaterialParameters.hpp"
#include <glm/fwd.hpp>
#include <string>
#include <utility>

namespace gk::gfx {

std::span<const std::pair<std::string, bool>> MockParameters::boolParameters() const noexcept {
  return {};
};

std::span<const std::pair<std::string, glm::int32>> MockParameters::intParameters() const noexcept {
  return {};
};

std::span<const std::pair<std::string, glm::float32>> MockParameters::floatParameters()
    const noexcept {
  return {};
};

std::span<const std::pair<std::string, glm::vec2>> MockParameters::vec2Parameters() const noexcept {
  return {};
};

std::span<const std::pair<std::string, glm::vec3>> MockParameters::vec3Parameters() const noexcept {
  return {};
};

std::span<const std::pair<std::string, glm::vec4>> MockParameters::vec4Parameters() const noexcept {
  return {};
};

std::span<const std::pair<std::string, glm::mat3>> MockParameters::mat3Parameters() const noexcept {
  return {};
};

std::vector<std::pair<std::string, glm::mat4>> MockParameters::mat4Parameters() const noexcept {
  return {};
};

PhongMaterialParams::PhongMaterialParams() {
  // copper by default
  m_vecParams[0] = {"material.ambient", glm::vec3(0.19125, 0.0735, 0.0225)};
  m_vecParams[1] = {"material.diffuse", glm::vec3(0.7038, 0.27048, 0.0828)};
  m_vecParams[2] = {"material.specular", glm::vec3(0.256777, 0.137622, 0.086014)};
  m_shininessParam = std::pair{"material.shininess", 0.1};
}

PhongMaterialParams::PhongMaterialParams(const glm::vec3& ambient, const glm::vec3& diffuse,
                                         const glm::vec3& specular, glm::float32 shininess) {
  m_vecParams[0] = {"material.ambient", ambient};
  m_vecParams[1] = {"material.diffuse", diffuse};
  m_vecParams[2] = {"material.specular", specular};
  m_shininessParam = std::pair{"material.shininess", shininess};
}

std::span<const std::pair<std::string, bool>> PhongMaterialParams::boolParameters() const noexcept {
  return {};
};

std::span<const std::pair<std::string, glm::int32>> PhongMaterialParams::intParameters()
    const noexcept {
  return {};
};

std::span<const std::pair<std::string, glm::float32>> PhongMaterialParams::floatParameters()
    const noexcept {
  return std::span(&m_shininessParam, 1);
}

std::span<const std::pair<std::string, glm::vec2>> PhongMaterialParams::vec2Parameters()
    const noexcept {
  return {};
};

std::span<const std::pair<std::string, glm::vec3>> PhongMaterialParams::vec3Parameters()
    const noexcept {
  return m_vecParams;
}

std::span<const std::pair<std::string, glm::vec4>> PhongMaterialParams::vec4Parameters()
    const noexcept {
  return {};
};

std::span<const std::pair<std::string, glm::mat3>> PhongMaterialParams::mat3Parameters()
    const noexcept {
  return {};
};

std::vector<std::pair<std::string, glm::mat4>> PhongMaterialParams::mat4Parameters()
    const noexcept {
  return {};
};

void PhongMaterialParams::setParameter(const std::string& key, const glm::vec3 value) noexcept {
  if (key == "material.ambient") {
    m_vecParams[0].second = value;
  } else if (key == "material.diffuse") {
    m_vecParams[1].second = value;
  } else if (key == "material.specular") {
    m_vecParams[2].second = value;
  }
}
void PhongMaterialParams::setParameter(const std::string& key, const glm::float32 value) noexcept {
  if (key == "material.shininess") {
    m_shininessParam.second = value;
  }
}

PhongMaterialParamsAnimated::PhongMaterialParamsAnimated(
    std::unique_ptr<animation::Skeleton>&& skeleton) {
  m_skel = std::move(skeleton);
  m_numBones = std::pair{"num_bones", m_skel->size()};
  for (int i = 0; i < m_skel->size(); i++) {
    m_bonesParams.push_back(std::pair{"bones[" + std::to_string(i) + "]", (*m_skel)[i].tr});
  }
}

std::span<const std::pair<std::string, glm::int32>> PhongMaterialParamsAnimated::intParameters()
    const noexcept {
  return std::span{&m_numBones, 1};
};

animation::Skeleton& PhongMaterialParamsAnimated::skeleton() { return *m_skel; }

std::vector<std::pair<std::string, glm::mat4>> PhongMaterialParamsAnimated::mat4Parameters()
    const noexcept {
  std::vector<std::pair<std::string, glm::mat4>> params = m_bonesParams;
  for (int i = 0; i < m_skel->size(); i++) {
    params[i].second = (*m_skel)[i].tr;
  }
  return params;
}

}  // namespace gk::gfx
