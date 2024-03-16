/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <span>
#include <string>

#include "Animation/Skeleton.hpp"

// TODO: refactor this and add textures here

namespace gk::gfx {
class MaterialParameters {
 public:
  MaterialParameters(){};
  MaterialParameters(const MaterialParameters&) = default;
  MaterialParameters(MaterialParameters&&) = default;
  MaterialParameters& operator=(const MaterialParameters&) = default;
  MaterialParameters& operator=(MaterialParameters&&) = default;

  virtual ~MaterialParameters(){};

  virtual void setParameter(const std::string&, bool) noexcept = 0;
  virtual void setParameter(const std::string&, glm::int32) noexcept = 0;
  virtual void setParameter(const std::string&, glm::float32) noexcept = 0;
  virtual void setParameter(const std::string&, const glm::vec2) noexcept = 0;
  virtual void setParameter(const std::string&, const glm::vec3) noexcept = 0;
  virtual void setParameter(const std::string&, const glm::vec4) noexcept = 0;
  virtual void setParameter(const std::string&, const glm::mat3) noexcept = 0;
  virtual void setParameter(const std::string&, const glm::mat4) noexcept = 0;

  virtual std::span<const std::pair<std::string, bool>> boolParameters() const noexcept = 0;
  virtual std::span<const std::pair<std::string, glm::int32>> intParameters() const noexcept = 0;
  virtual std::span<const std::pair<std::string, glm::float32>> floatParameters()
      const noexcept = 0;
  virtual std::span<const std::pair<std::string, glm::vec2>> vec2Parameters() const noexcept = 0;
  virtual std::span<const std::pair<std::string, glm::vec3>> vec3Parameters() const noexcept = 0;
  virtual std::span<const std::pair<std::string, glm::vec4>> vec4Parameters() const noexcept = 0;
  virtual std::span<const std::pair<std::string, glm::mat3>> mat3Parameters() const noexcept = 0;
  virtual std::vector<std::pair<std::string, glm::mat4>> mat4Parameters() const noexcept = 0;
};

class MockParameters : public MaterialParameters {
 public:
  MockParameters(){};
  MockParameters(const MockParameters&) = default;
  MockParameters(MockParameters&&) = default;
  MockParameters& operator=(const MockParameters&) = default;
  MockParameters& operator=(MockParameters&&) = default;

  void setParameter(const std::string&, bool) noexcept override{};
  void setParameter(const std::string&, glm::int32) noexcept override{};
  void setParameter(const std::string&, glm::float32) noexcept override{};
  void setParameter(const std::string&, const glm::vec2) noexcept override{};
  void setParameter(const std::string&, const glm::vec3) noexcept override{};
  void setParameter(const std::string&, const glm::vec4) noexcept override{};
  void setParameter(const std::string&, const glm::mat3) noexcept override{};
  void setParameter(const std::string&, const glm::mat4) noexcept override{};

  std::span<const std::pair<std::string, bool>> boolParameters() const noexcept override;
  std::span<const std::pair<std::string, glm::int32>> intParameters() const noexcept override;
  std::span<const std::pair<std::string, glm::float32>> floatParameters() const noexcept override;
  std::span<const std::pair<std::string, glm::vec2>> vec2Parameters() const noexcept override;
  std::span<const std::pair<std::string, glm::vec3>> vec3Parameters() const noexcept override;
  std::span<const std::pair<std::string, glm::vec4>> vec4Parameters() const noexcept override;
  std::span<const std::pair<std::string, glm::mat3>> mat3Parameters() const noexcept override;
  std::vector<std::pair<std::string, glm::mat4>> mat4Parameters() const noexcept override;
};

class PhongMaterialParams : public MaterialParameters {
 public:
  PhongMaterialParams();
  PhongMaterialParams(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
                      glm::float32 shininess);

  void setParameter(const std::string&, bool) noexcept override{};
  void setParameter(const std::string&, glm::int32) noexcept override{};
  void setParameter(const std::string&, glm::float32) noexcept override;
  void setParameter(const std::string&, const glm::vec2) noexcept override{};
  void setParameter(const std::string&, const glm::vec3) noexcept override;
  void setParameter(const std::string&, const glm::vec4) noexcept override{};
  void setParameter(const std::string&, const glm::mat3) noexcept override{};
  void setParameter(const std::string&, const glm::mat4) noexcept override{};

  std::span<const std::pair<std::string, bool>> boolParameters() const noexcept override;
  std::span<const std::pair<std::string, glm::int32>> intParameters() const noexcept override;
  std::span<const std::pair<std::string, glm::float32>> floatParameters() const noexcept override;
  std::span<const std::pair<std::string, glm::vec2>> vec2Parameters() const noexcept override;
  std::span<const std::pair<std::string, glm::vec3>> vec3Parameters() const noexcept override;
  std::span<const std::pair<std::string, glm::vec4>> vec4Parameters() const noexcept override;
  std::span<const std::pair<std::string, glm::mat3>> mat3Parameters() const noexcept override;
  std::vector<std::pair<std::string, glm::mat4>> mat4Parameters() const noexcept override;

 private:
  std::array<std::pair<std::string, glm::vec3>, 3> m_vecParams;
  std::pair<std::string, glm::float32> m_shininessParam;
};

class PhongMaterialParamsAnimated : public PhongMaterialParams {
 public:
  PhongMaterialParamsAnimated(std::unique_ptr<animation::Skeleton>&& skeleton);
  std::vector<std::pair<std::string, glm::mat4>> mat4Parameters() const noexcept override;
  std::span<const std::pair<std::string, glm::int32>> intParameters() const noexcept override;

  animation::Skeleton& skeleton();

 private:
  std::unique_ptr<animation::Skeleton> m_skel;
  std::pair<std::string, glm::int32> m_numBones;
  std::vector<std::pair<std::string, glm::mat4>> m_bonesParams;
};


class MetallicRoughnessMaterialParams : public MockParameters {
 public:
  MetallicRoughnessMaterialParams();
  MetallicRoughnessMaterialParams(const glm::vec4& baseColorFactor, float roughnessFactor, float metallicFactor);

  void setParameter(const std::string&, glm::float32) noexcept override;
  void setParameter(const std::string&, const glm::vec4) noexcept override;

  std::span<const std::pair<std::string, glm::float32>> floatParameters() const noexcept override;
  std::span<const std::pair<std::string, glm::vec4>> vec4Parameters() const noexcept override;

 private:
  std::pair<std::string, glm::vec4> m_baseColorFactor;
  std::array<std::pair<std::string, glm::float32>, 2> m_floatParams;
};

}  // namespace gk::gfx
