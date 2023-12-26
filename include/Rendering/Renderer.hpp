/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <GL/glew.h>

#include <memory>
#include <vector>

#include "../Geometry/Mesh.hpp"
#include "RenderObject.hpp"
#include "Scene.hpp"
#include "ShaderProgram.hpp"

class Renderer {
   public:
    Renderer(std::shared_ptr<AssetManager> assetManager);
    void resize(int width, int height);
    void setViewport(int x, int y, int width, int height);
    const Scene& getScene() const;
    Scene& getScene();
    void renderScene() const;
    const std::shared_ptr<ShaderProgram> getProgram(const std::string& name) const;

   private:
    void compileShaders();
    Scene m_scene;
    std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> m_programs;
    std::shared_ptr<AssetManager> m_asset_manager;
    float m_aspectRatio;
};
