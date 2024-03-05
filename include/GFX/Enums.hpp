/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

namespace gk::gfx {
enum class VSyncMode { eDisabled, eDoubleBuffering, eTripleBuffering };

enum class TextureWrapping {
  REPEAT,
  MIRROR,
  EDGE,
  BORDER,
};

enum class TextureFiltering { NEAREST, LINEAR };

}  // namespace gk::gfx
