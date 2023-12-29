/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <GL/glew.h>

#include <memory>

#include "GFX/OpenGL/GLShaderProgram.hpp"
#include "IO/RessourceManager.hpp"
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
  const std::shared_ptr<gfx::gl::GLShaderProgram> getProgram(const std::string& name) const;

 private:
  void compileShaders();
  Scene m_scene;
  std::unordered_map<std::string, std::shared_ptr<gfx::gl::GLShaderProgram>> m_programs;
  std::shared_ptr<io::RessourceManager> m_ressourceManager;
  float m_aspectRatio;
};

}  // namespace gk::rendering
