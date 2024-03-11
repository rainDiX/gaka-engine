/*
 * SPDX-License-Identifier: MIT
 */

#include <memory>

#include "GFX/OpenGL/GLTexture.hpp"
#include "Rendering/SceneNodes.hpp"

namespace gk::rendering {

TextureNode::TextureNode(long id, const std::span<std::byte> texture, int width, int height)
    : SceneNode(id) {
  m_tex = std::make_unique<gfx::gl::Texture>(texture, width, height);
}

void TextureNode::update(const std::span<std::byte> texture, int width, int height) noexcept {
  m_tex = std::make_unique<gfx::gl::Texture>(texture, width, height);
}

void TextureNode::bind() noexcept { m_tex->bind(); }

NodeType TextureNode::nodeType() const { return NodeType::eTexture; }

const gfx::gl::Texture& TextureNode::texture() const noexcept { return *m_tex; }
}  // namespace gk::rendering
