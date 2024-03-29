#pragma once

#include "VulkanDevice.hpp"

// vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <memory>
#include <string>
#include <vector>

namespace NK {

class VulkanSwapChain {
 public:
  static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

  VulkanSwapChain(VulkanDevice* deviceRef, VkExtent2D windowExtent);
  VulkanSwapChain(VulkanDevice* deviceRef, VkExtent2D windowExtent, std::shared_ptr<VulkanSwapChain> previous);
  ~VulkanSwapChain();

  VulkanSwapChain(const VulkanSwapChain &) = delete;
  VulkanSwapChain& operator=(const VulkanSwapChain &) = delete;

  VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
  VkRenderPass getRenderPass() { return renderPass; }
  VkImageView getImageView(int index) { return swapChainImageViews[index]; }
  size_t imageCount() { return swapChainImages.size(); }
  VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
  VkExtent2D getSwapChainExtent() { return swapChainExtent; }
  uint32_t width() { return swapChainExtent.width; }
  uint32_t height() { return swapChainExtent.height; }

  float extentAspectRatio() {
    float ret = static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
    NK_CORE_TRACE("extension = {0}, {1}; ret = {2}", swapChainExtent.width, swapChainExtent.height, ret);
    return ret;
  }
  VkFormat findDepthFormat();

  VkResult acquireNextImage(uint32_t *imageIndex);
  VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

  bool compareSwapFormat(const VulkanSwapChain& swapChain) const
  {
    return swapChain.swapchainDepthFormat == swapchainDepthFormat && 
           swapChain.swapChainImageFormat == swapChainImageFormat;
  }

 private:
  void init();
  void createSwapChain();
  void createImageViews();
  void createDepthResources();
  void createRenderPass();
  void createFramebuffers();
  void createSyncObjects();

  // Helper functions
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

  VkFormat swapChainImageFormat;
  VkFormat swapchainDepthFormat;
  VkExtent2D swapChainExtent;

  std::vector<VkFramebuffer> swapChainFramebuffers;
  VkRenderPass renderPass;

  std::vector<VkImage> depthImages;
  std::vector<VkDeviceMemory> depthImageMemorys;
  std::vector<VkImageView> depthImageViews;
  std::vector<VkImage> swapChainImages;
  std::vector<VkImageView> swapChainImageViews;

  VulkanDevice* device = nullptr;
  VkExtent2D windowExtent;

  VkSwapchainKHR swapChain;
  std::shared_ptr<VulkanSwapChain> oldSwapchain;

  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  std::vector<VkFence> imagesInFlight;
  size_t currentFrame = 0;
};

}  // namespace lve
