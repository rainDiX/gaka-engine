/*
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <vector>

#include "GFX/OpenGL/GLMesh.hpp"
#include "GFX/OpenGL/GLTexture.hpp"
#include "Rendering/RenderObject.hpp"

namespace gk::rendering {
class Model : public RenderObject {
 public:
  Model(std::string_view path) { loadModel(path); }

 private:
  void loadModel(std::string_view path);
  void processNode(aiNode *node, const aiScene *scene);
  gfx::gl::Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<gfx::gl::Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                                std::string typeName);
};

}  // namespace gk::rendering