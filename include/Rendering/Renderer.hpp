/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <epoxy/gl.h>

#include <memory>
#include <vector>

#include "GFX/OpenGL/GLShaderProgram.hpp"
#include "IO/RessourceManager.hpp"
#include "Rendering/SceneNodes.hpp"
#include "Scene.hpp"

namespace gk::rendering {
class Renderer {
 public:
  Renderer(std::shared_ptr<io::RessourceManager> assetManager);
  void resize(int width, int height);
  void setViewport(int x, int y, int width, int height);
  const Scene& getScene() const;
  Scene& getScene();
  void renderScene() const;
  const std::shared_ptr<gfx::gl::ShaderProgram> getProgram(const std::string& name) const;

 private:
  void renderNode(SceneNode* node, const glm::mat4& projection, const gfx::FlyingCamera& camera,
                  std::vector<LightNode*> lights = {}) const;

  Scene m_scene{};
  std::shared_ptr<io::RessourceManager> m_ressourceManager;
  float m_aspectRatio;
};

}  // namespace gk::rendering
