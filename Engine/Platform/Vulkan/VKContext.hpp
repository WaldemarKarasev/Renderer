#pragma once

#include "vulkan/vulkan.h"
#include <vector>

#include "VulkanDevice.hpp"

namespace NK {


struct GPUInfo_t 
{
    VkPhysicalDevice						device;
    VkPhysicalDeviceProperties				props;
    VkPhysicalDeviceMemoryProperties		memProps;
    VkPhysicalDeviceFeatures				features;
    VkSurfaceCapabilitiesKHR				surfaceCaps;
    std::vector< VkSurfaceFormatKHR >		surfaceFormats;
    std::vector< VkPresentModeKHR >			presentModes;
    std::vector< VkQueueFamilyProperties >	queueFamilyProps;
    std::vector< VkExtensionProperties >	extensionProps;
};

struct GlobalVulkanContext
{
    GPUInfo_t gpu;
    VkDevice  device;
};
GlobalVulkanContext vkContext{};

struct VulkanRenderBackendContext
{
    VkDescriptorSetLayout globalDescriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets;
};

struct VulkanCurrentFrameContext
{
    VkCommandBuffer CurrentCommandBuffer = nullptr;
};


}
