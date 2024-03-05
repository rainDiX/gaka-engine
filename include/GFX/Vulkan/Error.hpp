/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <optional>
#include <vulkan/vulkan.hpp>

namespace gk::gfx::vulkan {

struct Error {
  std::optional<vk::Result> vkerror;
  std::string msg;
};

};  // namespace gk::gfx::vulkan
