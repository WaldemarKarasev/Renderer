#pragma once


// Vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <string>
#include <vector>

// Engine headers
#include "Nutckracker/Window.hpp"

namespace NK {

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
  uint32_t graphicsFamily;
  uint32_t presentFamily;
  bool graphicsFamilyHasValue = false;
  bool presentFamilyHasValue = false;
  bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

class VulkanDevice {
 public:
#ifdef NDEBUG
  const bool enableValidationLayers = false;
#else
  const bool enableValidationLayers = true;
#endif

  VulkanDevice(Window* window);
  ~VulkanDevice();

  // Not copyable or movable
  VulkanDevice(const VulkanDevice&) = delete;
  VulkanDevice& operator=(const VulkanDevice&) = delete;
  VulkanDevice(VulkanDevice&&) = delete;
  VulkanDevice &operator=(VulkanDevice&&) = delete;

  VkCommandPool getCommandPool() { return m_CommandPool_; }
  VkDevice device() { return m_Device_; }
  VkSurfaceKHR surface() { return m_Surface_; }
  VkQueue graphicsQueue() { return m_GraphicsQueue_; }
  VkQueue presentQueue() { return m_PresentQueue_; }

  SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(m_PhysicalDevice_); }
  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
  QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(m_PhysicalDevice_); }
  VkFormat findSupportedFormat(
      const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

  // Buffer Helper Functions
  void createBuffer(
      VkDeviceSize size,
      VkBufferUsageFlags usage,
      VkMemoryPropertyFlags properties,
      VkBuffer &buffer,
      VkDeviceMemory &bufferMemory);
  VkCommandBuffer beginSingleTimeCommands();
  void endSingleTimeCommands(VkCommandBuffer commandBuffer);
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
  void copyBufferToImage(
      VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

  void createImageWithInfo(
      const VkImageCreateInfo &imageInfo,
      VkMemoryPropertyFlags properties,
      VkImage &image,
      VkDeviceMemory &imageMemory);

  VkPhysicalDeviceProperties m_Properties_;

 private:
  void createInstance();
  void setupDebugMessenger();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createCommandPool();

  // helper functions
  bool isDeviceSuitable(VkPhysicalDevice device);
  std::vector<const char *> getRequiredExtensions();
  bool checkValidationLayerSupport();
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
  void hasGflwRequiredInstanceExtensions();
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

  VkInstance m_Instance_;
  VkDebugUtilsMessengerEXT m_DebugMessenger_;
  VkPhysicalDevice m_PhysicalDevice_ = VK_NULL_HANDLE;
  VkCommandPool m_CommandPool_;

  Window* m_Window_ = nullptr;

  VkDevice m_Device_;
  VkSurfaceKHR m_Surface_;
  VkQueue m_GraphicsQueue_;
  VkQueue m_PresentQueue_;

  const std::vector<const char *> m_ValidationLayers_ = {"VK_LAYER_KHRONOS_validation"};
  const std::vector<const char *> m_DeviceExtensions_ = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};

}  // namespace lve
