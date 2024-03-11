/*
 * SPDX-License-Identifier: MIT
 */

#include "Rendering/SceneNodes.hpp"

namespace gk::rendering {

CameraNode::CameraNode(long id, gfx::FlyingCamera&& cam) : SceneNode(id) {
    m_camera = std::make_unique<gfx::FlyingCamera>(std::move(cam));
}

NodeType CameraNode::nodeType() const { return NodeType::eCamera; };

const gfx::FlyingCamera& CameraNode::camera() const { return *m_camera; }

gfx::FlyingCamera& CameraNode::camera() { return *m_camera; }

}  // namespace gk::rendering
