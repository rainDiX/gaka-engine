#include "Rendering/Scene.hpp"

#include <memory>
#include <optional>
#include <utility>

#include "Rendering/RenderObject.hpp"

namespace gk::rendering {
Scene::Scene()
    : m_camera(FlyingCamera(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 2.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f))) {}

void Scene::addPointLight(const PointLight& light, const glm::vec3& position) {
    m_pointLights.push_back(std::make_pair(light, position));
}

const std::vector<std::pair<PointLight, glm::vec3>>& Scene::pointLights() const { return m_pointLights; }

void Scene::addObject(const std::string& handle, std::unique_ptr<RenderObject>&& object) {
    m_objects.insert(std::make_pair(handle, std::move(object)));
}

std::optional<std::reference_wrapper<RenderObject>> Scene::getObject(const std::string& handle) {
    auto it = m_objects.find(handle);
    if (it != m_objects.end()) {
        return std::optional<std::reference_wrapper<RenderObject>>{*it->second};
    } else {
        return std::nullopt;
    }
}

void Scene::removeObject(const std::string& handle) { m_objects.erase(handle); }

const std::unordered_map<std::string, std::unique_ptr<RenderObject>>& Scene::objects() const { return m_objects; }

const FlyingCamera& Scene::camera() const { return m_camera; }

FlyingCamera& Scene::camera() { return m_camera; }

}  // namespace gk::rendering
