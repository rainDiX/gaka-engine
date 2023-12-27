/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <GL/glew.h>

#include <memory>

#include "GPU/OpenGL/GLShaderProgram.hpp"
#include "IO/AssetManager.hpp"
#include "Scene.hpp"

namespace gk::rendering {
class Renderer {
   public:
    Renderer(std::shared_ptr<io::AssetManager> assetManager);
    void resize(int width, int height);
    void setViewport(int x, int y, int width, int height);
    const Scene& getScene() const;
    Scene& getScene();
    void renderScene() const;
    const std::shared_ptr<gpu::gl::GLShaderProgram> getProgram(const std::string& name) const;

   private:
    void compileShaders();
    Scene m_scene;
    std::unordered_map<std::string, std::shared_ptr<gpu::gl::GLShaderProgram>> m_programs;
    std::shared_ptr<io::AssetManager> m_asset_manager;
    float m_aspectRatio;
};

}  // namespace gk::rendering
