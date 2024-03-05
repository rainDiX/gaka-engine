/*
 * SPDX-License-Identifier: MIT
 */

#include "GFX/Vulkan/VulkanManager.hpp"

#include <__expected/expected.h>
#include <__expected/unexpected.h>
#include <vulkan/vulkan_core.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_structs.hpp>

#include "GFX/Enums.hpp"
#include "GFX/Vulkan/Error.hpp"
#include "GUI/Window.hpp"

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageTypes,
              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* /*pUserData*/) {
  std::ostringstream message;

  message << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity))
          << ": " << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes))
          << ":\n";
  message << std::string("\t") << "messageIDName   = <" << pCallbackData->pMessageIdName << ">\n";
  message << std::string("\t") << "messageIdNumber = " << pCallbackData->messageIdNumber << "\n";
  message << std::string("\t") << "message         = <" << pCallbackData->pMessage << ">\n";
  if (0 < pCallbackData->queueLabelCount) {
    message << std::string("\t") << "Queue Labels:\n";
    for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++) {
      message << std::string("\t\t") << "labelName = <" << pCallbackData->pQueueLabels[i].pLabelName
              << ">\n";
    }
  }
  if (0 < pCallbackData->cmdBufLabelCount) {
    message << std::string("\t") << "CommandBuffer Labels:\n";
    for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
      message << std::string("\t\t") << "labelName = <"
              << pCallbackData->pCmdBufLabels[i].pLabelName << ">\n";
    }
  }
  if (0 < pCallbackData->objectCount) {
    message << std::string("\t") << "Objects:\n";
    for (uint32_t i = 0; i < pCallbackData->objectCount; i++) {
      message << std::string("\t\t") << "Object " << i << "\n";
      message << std::string("\t\t\t") << "objectType   = "
              << vk::to_string(static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType))
              << "\n";
      message << std::string("\t\t\t")
              << "objectHandle = " << pCallbackData->pObjects[i].objectHandle << "\n";
      if (pCallbackData->pObjects[i].pObjectName) {
        message << std::string("\t\t\t") << "objectName   = <"
                << pCallbackData->pObjects[i].pObjectName << ">\n";
      }
    }
  }

  std::cerr << message.str() << std::endl;

  return VK_FALSE;
}

namespace gk::gfx::vulkan {
std::expected<void, Error> VulkanManager::initialize(
    std::span<const char*> extensions, const gk::gui::WindowState* windowState,
    std::function<void(VkInstance, VkSurfaceKHR*)> createSurface,
    std::optional<std::span<const char*>> layers) noexcept {
  m_windowState = windowState;
  m_availableLayers = m_context.enumerateInstanceLayerProperties();

  m_extensions.assign(extensions.begin(), extensions.end());
  if (layers) {
    m_layers.assign(layers->begin(), layers->end());
  }

#ifdef NDEBUG
#else
  m_enableValidationLayers = true;
  m_layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

  for (const auto& layer : m_layers) {
    if (!checkLayerSupport(layer)) {
      return std::unexpected{Error{{}, std::format("{} Unsupported", layer)}};
    }
  }

  if (m_enableValidationLayers) {
    std::vector<vk::ExtensionProperties> props = m_context.enumerateInstanceExtensionProperties();

    auto propsIterator =
        std::find_if(props.begin(), props.end(), [](vk::ExtensionProperties const& ep) {
          return strcmp(ep.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0;
        });
    if (propsIterator == props.end()) {
      return std::unexpected{
          Error{{}, std::format("Cannot find {} extension", VK_EXT_DEBUG_UTILS_EXTENSION_NAME)}};
    }
    m_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  if (auto instance = createInstance(); instance.has_value()) {
    m_instance = std::move(*instance);
  } else {
    return std::unexpected{Error{instance.error(), "Failed to create instance"}};
  }

  {
    VkSurfaceKHR _surface;
    createSurface(static_cast<VkInstance>(*m_instance), &_surface);
    m_surface = vk::raii::SurfaceKHR(m_instance, std::move(_surface));
  }

  if (m_enableValidationLayers) {
    if (auto debugMessenger = createDebugMessenger(); debugMessenger.has_value()) {
      m_debugUtilsMessenger = std::move(*debugMessenger);
    } else {
      return std::unexpected{Error{debugMessenger.error(), "Failed to create DebugMessenger"}};
    }
  }

  if (auto devices = m_instance.enumeratePhysicalDevices(); devices.has_value()) {
    m_physicalDevices = std::move(*devices);
  } else {
    return std::unexpected{Error{devices.error(), "Failed to enumerate devices"}};
  }

  if ( auto graphicDevice = pickPhysicalDevice(); graphicDevice!= nullptr) {
    m_physicalDevice = graphicDevice->phy;
    m_graphicsQueueFamilyIndex = *graphicDevice->graphicsQueueFamilyIndex;
    m_presentQueueFamilyIndex = *graphicDevice->presentQueueFamilyIndex;
  } else {
    return std::unexpected{Error{{}, "No suitable device"}};
  }

  if (auto device = createLogicalDevice(m_graphicsQueueFamilyIndex); device.has_value()) {
    m_device = std::move(*device);
  } else {
    return std::unexpected{
        Error{device.error(), "Failed to create logical device and graphics queue"}};
  }

  if (auto graphicsQueue = m_device.getQueue(m_graphicsQueueFamilyIndex, 0);
      graphicsQueue.has_value()) {
    m_graphicsQueue = std::move(*graphicsQueue);
  } else {
    return std::unexpected{Error{graphicsQueue.error(), "Failed to get graphics queue"}};
  }

  if (auto presentQueue = m_device.getQueue(m_presentQueueFamilyIndex, 0);
      presentQueue.has_value()) {
    m_presentQueue = std::move(*presentQueue);
  } else {
    return std::unexpected{Error{presentQueue.error(), "Failed to get present queue"}};
  }

  if (auto swapchain = createSwapchain(); swapchain.has_value()) {
    m_swapchain = std::move(*swapchain);
  } else {
    return std::unexpected{swapchain.error()};
  }
  m_swapchainImages = m_swapchain.getImages();

  if (auto res = createImageViews(); !res.has_value()) {
    return std::unexpected{Error{res.error(), "Failed to create image views"}};
  }
  return {};
}

std::expected<vk::raii::Instance, vk::Result> VulkanManager::createInstance() const noexcept {
  vk::ApplicationInfo appInfo{
      "Gaka App", 1, "Gaka", 1, vk::ApiVersion13,
  };

  vk::InstanceCreateInfo createInfo({}, &appInfo, m_layers.size(), m_layers.data(),
                                    m_extensions.size(), m_extensions.data());

  return m_context.createInstance(createInfo);
}

std::expected<vk::raii::DebugUtilsMessengerEXT, vk::Result> VulkanManager::createDebugMessenger()
    const noexcept {
  vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
  vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(
      vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
      vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
  vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT(
      {}, severityFlags, messageTypeFlags, &debugCallback);
  return m_instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
}

bool VulkanManager::checkLayerSupport(const char* layerName) const noexcept {
  auto layerIterator = std::find_if(
      m_availableLayers.begin(), m_availableLayers.end(),
      [&](const vk::LayerProperties& layer) { return strcmp(layer.layerName, layerName) == 0; });
  return layerIterator != m_availableLayers.end();
};

std::unique_ptr<PhysicalDeviceInfo> getDeviceInfo(vk::raii::PhysicalDevice& device,
                                                  const vk::SurfaceKHR& surface) {
  auto queueFamilyProps = device.getQueueFamilyProperties();
  auto devInfo = std::make_unique<PhysicalDeviceInfo>(&device);

  uint32_t i = 0;
  while (i < queueFamilyProps.size() && (!devInfo->graphicsQueueFamilyIndex.has_value() ||
                                         !devInfo->presentQueueFamilyIndex.has_value())) {
    if (static_cast<bool>(queueFamilyProps[i].queueFlags & vk::QueueFlagBits::eGraphics)) {
      devInfo->graphicsQueueFamilyIndex = i;
    }
    if (device.getSurfaceSupportKHR(i, surface)) {
      devInfo->presentQueueFamilyIndex = i;
    }
    ++i;
  }
  return devInfo;
}

bool checkExtensionSupport(const vk::raii::PhysicalDevice& phy,
                           std::span<const char* const> requiredDeviceExtensions) {
  auto supportedExtensions = phy.enumerateDeviceExtensionProperties();
  bool supported = true;
  for (const auto& requiredExtension : requiredDeviceExtensions) {
    auto extIterator = std::find_if(supportedExtensions.begin(), supportedExtensions.begin(),
                                    [&](const vk::ExtensionProperties& ext) {
                                      return strcmp(ext.extensionName, requiredExtension) == 0;
                                    });
    supported = supported && (extIterator != supportedExtensions.end());
  }

  return supported;
}

std::unique_ptr<PhysicalDeviceInfo> VulkanManager::pickPhysicalDevice() noexcept {
  bool suitableDeviceFound = false;
  auto it = m_physicalDevices.begin();
  std::unique_ptr<PhysicalDeviceInfo> dev = nullptr;
  while (!suitableDeviceFound && it != m_physicalDevices.end()) {
    auto current = getDeviceInfo(*it, *m_surface);
    if (current->graphicsQueueFamilyIndex.has_value() &&
        current->presentQueueFamilyIndex.has_value() &&
        checkExtensionSupport(
            *it, {m_requiredDeviceExtensions.data(), m_requiredDeviceExtensions.size()})) {
      dev = std::move(current);
    }
    ++it;
  }
  return dev;
}

std::expected<vk::raii::Device, vk::Result> VulkanManager::createLogicalDevice(
    uint32_t graphicsQueueFamilyIndex) const noexcept {
  float queuePriority = 1.0f;
  vk::DeviceQueueCreateInfo devQueueCreateInfo({}, graphicsQueueFamilyIndex, 1, &queuePriority);
  vk::DeviceCreateInfo deviceCreateInfo({}, devQueueCreateInfo, {}, m_requiredDeviceExtensions);
  return m_physicalDevice->createDevice(deviceCreateInfo);
}

vk::PresentModeKHR chooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR>& availablePresentModes, gfx::VSyncMode vsync) {
  for (const auto& availablePresentMode : availablePresentModes) {
    if (vsync == VSyncMode::eTripleBuffering &&
        availablePresentMode == vk::PresentModeKHR::eMailbox) {
      return availablePresentMode;
    }
    if (vsync == VSyncMode::eDisabled && availablePresentMode == vk::PresentModeKHR::eImmediate) {
      return availablePresentMode;
    }
  }
  // only FIFO is requiered to be present by the spec
  return vk::PresentModeKHR::eFifo;
}

std::expected<vk::raii::SwapchainKHR, Error> VulkanManager::createSwapchain() noexcept {
  std::vector<vk::SurfaceFormatKHR> availableFormats =
      m_physicalDevice->getSurfaceFormatsKHR(*m_surface);
  if (availableFormats.empty()) {
    return std::unexpected{Error{{}, "No format found for surface"}};
  }

  // Init the swap chain with an sRGB color depth
  for (auto& availableFormat : availableFormats) {
    if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
        availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
      m_swapchainImageFormat = availableFormat.format;
    }
  }
  if (m_swapchainImageFormat == vk::Format::eUndefined) {
    m_swapchainImageFormat = availableFormats[0].format;
  }

  vk::SurfaceCapabilitiesKHR surfaceCapabilities =
      m_physicalDevice->getSurfaceCapabilitiesKHR(*m_surface);
  if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max()) {
    // If the surface size is undefined, the size is set to the size of the images requested.
    m_swapchainExtent.width =
        std::clamp(m_windowState->width, surfaceCapabilities.minImageExtent.width,
                   surfaceCapabilities.maxImageExtent.width);
    m_swapchainExtent.height =
        std::clamp(m_windowState->height, surfaceCapabilities.minImageExtent.height,
                   surfaceCapabilities.maxImageExtent.height);
  } else {
    // If the surface size is defined, the swap chain size must match
    m_swapchainExtent = surfaceCapabilities.currentExtent;
  }

  auto availablePresentModes = m_physicalDevice->getSurfacePresentModesKHR(*m_surface);
  auto swapchainPresentMode = chooseSwapPresentMode(availablePresentModes, m_windowState->vsync);

  vk::SurfaceTransformFlagBitsKHR preTransform =
      (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity)
          ? vk::SurfaceTransformFlagBitsKHR::eIdentity
          : surfaceCapabilities.currentTransform;

  vk::CompositeAlphaFlagBitsKHR compositeAlpha =
      (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied)
          ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
      : (surfaceCapabilities.supportedCompositeAlpha &
         vk::CompositeAlphaFlagBitsKHR::ePostMultiplied)
          ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
      : (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit)
          ? vk::CompositeAlphaFlagBitsKHR::eInherit
          : vk::CompositeAlphaFlagBitsKHR::eOpaque;

  auto imageCount = surfaceCapabilities.minImageCount + 1;
  if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount) {
    imageCount = surfaceCapabilities.maxImageCount;
  }

  vk::SwapchainCreateInfoKHR swapChainCreateInfo(
      vk::SwapchainCreateFlagsKHR(), *m_surface, imageCount, m_swapchainImageFormat,
      vk::ColorSpaceKHR::eSrgbNonlinear, m_swapchainExtent, 1,
      vk::ImageUsageFlagBits::eColorAttachment, vk::SharingMode::eExclusive, {}, preTransform,
      compositeAlpha, swapchainPresentMode, true, nullptr);

  uint32_t queueFamilyIndices[] = {m_graphicsQueueFamilyIndex, m_presentQueueFamilyIndex};
  if (m_graphicsQueueFamilyIndex != m_presentQueueFamilyIndex) {
    swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
    swapChainCreateInfo.queueFamilyIndexCount = 2;
    swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
  }

  auto swapchain = m_device.createSwapchainKHR(swapChainCreateInfo);
  if (!swapchain.has_value()) {
    return std::unexpected{Error{swapchain.error(), "Failed to create swapchain"}};
  } else {
    return std::move(*swapchain);
  }
}

std::expected<void, vk::Result> VulkanManager::createImageViews() noexcept {
  m_imageViews.reserve(m_swapchainImages.size());
  vk::ImageViewCreateInfo imageViewCreateInfo({}, {}, vk::ImageViewType::e2D,
                                              m_swapchainImageFormat, {},
                                              {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});
  for (auto image : m_swapchainImages) {
    imageViewCreateInfo.image = image;
    auto imageView = m_device.createImageView(imageViewCreateInfo);
    if (imageView.has_value()) {
      m_imageViews.push_back(std::move(*imageView));
    } else {
      return std::unexpected{imageView.error()};
    }
  }
  return {};
}

std::expected<void, vk::Result> VulkanManager::createGraphicsPipeline() noexcept {
}

}  // namespace gk::gfx::vulkan
