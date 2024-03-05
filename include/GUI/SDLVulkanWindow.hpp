/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "GFX/Vulkan/VulkanManager.hpp"
#include "GUI/SDLWindow.hpp"

namespace gk::gui {
class SDLVulkanWindow : public SDLWindow {
 public:
  SDLVulkanWindow(const std::string& title, uint32_t width = 1280, uint32_t height = 720,
                  bool fullscreen = false, gfx::VSyncMode vsync = gfx::VSyncMode::eDisabled);
  SDLVulkanWindow() = delete;
  ~SDLVulkanWindow();

  SDLVulkanWindow(const SDLWindow& other) = delete;
  SDLVulkanWindow& operator=(const SDLVulkanWindow& other) = delete;

  void update() override;

 private:
  gfx::vulkan::VulkanManager* m_vkManager;
};

}  // namespace gk::gui
