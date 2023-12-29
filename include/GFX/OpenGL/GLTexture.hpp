/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <GL/glew.h>

#include <string>

namespace gk::gfx::gl {
enum GLTextureType {
  DIFFUSE,
  SPECULAR,
};

struct GLTexture {
  GLuint id;
  GLTextureType txType;
  std::string path;
};

}  // namespace gk::gfx::gl
