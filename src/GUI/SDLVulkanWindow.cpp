/**
 * SPDX-License-Identifier: MIT
 */

#include "GUI/SDLVulkanWindow.hpp"

#include <SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <iostream>
#include <span>
#include <stdexcept>
#include <vulkan/vulkan_to_string.hpp>

#include "GUI/Window.hpp"

namespace gk::gui {

SDLVulkanWindow::SDLVulkanWindow(const std::string& title, uint32_t width, uint32_t height,
                                 bool fullscreen, gfx::VSyncMode vsync) {
  m_state = {.width = width,
             .height = height,
             .fullscreen = fullscreen,
             .vsync = vsync,
             .mouseLocked = false,
             .mouseX = 0,
             .mouseY = 0};
  unsigned long flags = SDL_WINDOW_VULKAN;  // | SDL_WINDOW_RESIZABLE;
  if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

  // SDL: initialize and configure
  // ------------------------------
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "failed to init SDL2: " << SDL_GetError() << '\n';
    throw std::runtime_error("Failed to init SDL2");
  }

  // TODO Enable v-sync (set 1 to enable, 0 to disable)
  SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_WARP_MOTION, "1");

  // SDL window creation
  // --------------------
  m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              m_state.width, m_state.height, flags);

  if (m_window == NULL) {
    SDL_Quit();
    throw std::runtime_error("Failed to create SDL2 window");
  }

  // Vulkan init
  unsigned int extensionCount = 0;
  SDL_Vulkan_GetInstanceExtensions(m_window, &extensionCount, nullptr);
  std::vector<const char*> extensionNames(extensionCount);
  SDL_Vulkan_GetInstanceExtensions(m_window, &extensionCount, extensionNames.data());

  m_vkManager = new gfx::vulkan::VulkanManager();
  auto createSurface = [this](VkInstance instance, VkSurfaceKHR* surface) {
    SDL_Vulkan_CreateSurface(m_window, instance, surface);
  };

  if (auto res = m_vkManager->initialize(extensionNames, &m_state, createSurface);
      !res.has_value()) {
    delete m_vkManager;
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    std::string message = res.error().msg;
    if (res.error().vkerror) {
      message.append("\nVulkan Error: ");
      message.append(vk::to_string(*(res.error().vkerror)));
    }
    throw std::runtime_error(message);
  }
}

SDLVulkanWindow::~SDLVulkanWindow() {
  // Cleanup SDL2 resources
  delete m_vkManager;
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

void SDLVulkanWindow::update() {}

}  // namespace gk::gui
