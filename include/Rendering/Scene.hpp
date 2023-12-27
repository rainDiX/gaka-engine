/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "FlyingCamera.hpp"
#include "PointLight.hpp"
#include "RenderObject.hpp"

namespace gk::rendering {

class Scene {
   public:
    Scene();
    void addPointLight(const PointLight& light, const glm::vec3& position);
    const std::vector<std::pair<PointLight, glm::vec3>>& pointLights() const;
    void addObject(const std::string& handle, std::unique_ptr<RenderObject>&& object);
    std::optional<std::reference_wrapper<RenderObject>> getObject(const std::string& handle);
    void removeObject(const std::string& handle);
    const std::unordered_map<std::string, std::unique_ptr<RenderObject>>& objects() const;
    const FlyingCamera& camera() const;
    FlyingCamera& camera();

   private:
    FlyingCamera m_camera;
    std::unordered_map<std::string, std::unique_ptr<RenderObject>> m_objects;
    std::vector<std::pair<PointLight, glm::vec3>> m_pointLights;
};

}  // namespace gk::rendering
