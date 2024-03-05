/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <__expected/expected.h>
#include <cstdint>
#include <expected>
#include <functional>
#include <memory>
#include <optional>
#include <span>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_structs.hpp>

#include "GFX/Vulkan/Error.hpp"
#include "GUI/Window.hpp"

namespace gk::gfx::vulkan {

struct PhysicalDeviceInfo {
  vk::raii::PhysicalDevice* phy = nullptr;
  std::optional<uint32_t> graphicsQueueFamilyIndex = {};
  std::optional<uint32_t> presentQueueFamilyIndex = {};
};

class VulkanManager {
 public:
  VulkanManager() = default;
  ~VulkanManager() = default;
  VulkanManager(VulkanManager& other) = delete;
  VulkanManager& operator=(VulkanManager& other) = delete;

  /**
   * Initializes the VulkanManager.
   *
   * @param extensions an array of strings representing the required Vulkan extensions
   * @param windowState a pointer to the window state struct
   * @param createSurface a function that creates the Vulkan surface
   * @param layers an optional array of strings representing the Vulkan instance layers
   *
   * @return An `expected` value indicating success or failure
   *
   */
  std::expected<void, Error> initialize(
      std::span<const char*> extensions, const gk::gui::WindowState* windowState,
      std::function<void(VkInstance, VkSurfaceKHR*)> createSurface,
      std::optional<std::span<const char*>> layers = {}) noexcept;

 private:
  /**
   * Creates a Vulkan instance.
   *
   * @return An `expected` object containing a `vk::raii::Instance` if the operation is successful,
   * otherwise a `vk::Result`.
   *
   */
  std::expected<vk::raii::Instance, vk::Result> createInstance() const noexcept;

  /**
   * Creates a Vulkan debug messenger.
   *
   * @return An `expected` object containing a `vk::raii::DebugUtilsMessengerEXT` if the operation
   * is successful, otherwise a `vk::Result`.
   *
   */
  std::expected<vk::raii::DebugUtilsMessengerEXT, vk::Result> createDebugMessenger() const noexcept;

  /**
   * Checks if a Vulkan layer is supported.
   *
   * @param layerName The name of the Vulkan layer to check.
   *
   * @return True if the layer is supported, false otherwise.
   *
   */
  bool checkLayerSupport(const char* layerName) const noexcept;

  /**
   * Picks a suitable physical device for Vulkan operations.
   * A Device is considered suitable if it has both a graphics Queue and a present queue and
   * if the requiered extensions are supported
   *
   * @return A unique pointer to a PhysicalDeviceInfo object representing the picked physical
   * device or an `nullptr` if no suitable device is found.
   *
   */
  std::unique_ptr<PhysicalDeviceInfo> pickPhysicalDevice() noexcept;

  /**
   * Creates a logical device .
   *
   * @param graphicsQueueFamilyIndex the index of the graphics queue family
   *
   * @return an expected object containing a `vk::raii::Device` or a `vk::Result`
   *
   */
  std::expected<vk::raii::Device, vk::Result> createLogicalDevice(
      uint32_t graphicsQueueFamilyIndex) const noexcept;

  /**
   * Creates a swapchain for the VulkanManager.
   *
   * @param width The width of the swapchain.
   * @param height The height of the swapchain.
   *
   * @return An `expected` object containing the created `SwapchainKHR` or an `vk::Result` object if
   * an error occurs.
   *
   */
  std::expected<vk::raii::SwapchainKHR, Error> createSwapchain() noexcept;

  std::expected<void, vk::Result> createImageViews() noexcept;

  std::expected<void, vk::Result> createGraphicsPipeline() noexcept;

 private:
  vk::raii::Context m_context;
  vk::raii::Instance m_instance = VK_NULL_HANDLE;
  vk::raii::SurfaceKHR m_surface = VK_NULL_HANDLE;
  vk::raii::DebugUtilsMessengerEXT m_debugUtilsMessenger = VK_NULL_HANDLE;
  std::vector<vk::raii::PhysicalDevice> m_physicalDevices;
  vk::raii::PhysicalDevice* m_physicalDevice = nullptr;
  vk::raii::Device m_device = VK_NULL_HANDLE;
  vk::raii::Queue m_graphicsQueue = VK_NULL_HANDLE;
  vk::raii::Queue m_presentQueue = VK_NULL_HANDLE;
  vk::Format m_swapchainImageFormat = vk::Format::eUndefined;
  vk::Extent2D m_swapchainExtent;
  vk::raii::SwapchainKHR m_swapchain = VK_NULL_HANDLE;
  std::vector<vk::Image> m_swapchainImages;
  std::vector<vk::raii::ImageView> m_imageViews;

  const gk::gui::WindowState* m_windowState = nullptr;
  std::vector<vk::LayerProperties> m_availableLayers;
  const std::array<const char*, 1> m_requiredDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  std::vector<const char*> m_extensions;
  std::vector<const char*> m_layers;
  bool m_enableValidationLayers;
  uint32_t m_graphicsQueueFamilyIndex;
  uint32_t m_presentQueueFamilyIndex;
};
}  // namespace gk::gfx::vulkan
