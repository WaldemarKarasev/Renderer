#pragma once

#include "Nutckracker/Renderer/Pipeline.hpp"

#include <vulkan/vulkan.h>

#include "VulkanShader.hpp"
#include "VulkanRenderBackend.hpp"


// std
#include <string>
#include <vector>

namespace NK
{
    struct SimplePushConstantData
    {
        //glm::mat4 transform{1.f};
        //alignas(16) glm::vec3 color;
        glm::mat4 modelMatrix{1.f};
        glm::mat4 normalMatrix{1.f};
    };


    struct PointLightPushConstant
    {
        glm::vec4 position{};
        glm::vec4 coolor{};
        float radius{};
    }; 

    struct PipeLineConfigInfo 
    {
        PipeLineConfigInfo() = default;
        PipeLineConfigInfo(const PipeLineConfigInfo&) = delete;
        PipeLineConfigInfo& operator=(const PipeLineConfigInfo&) = delete;

        std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class VulkanPipeline : public Pipeline
    {
    public:
        //Me
        VulkanPipeline(Shader* shader);
        virtual void Init(SystemInfo& systemInfo) override;

        VulkanPipeline(Shader* shader, const PipeLineConfigInfo& configInfo);

        virtual void Bind() override;


        VulkanPipeline(
            VulkanDevice* device,
            const std::string& vertFilePath, 
            const std::string& fragFilePath,
            const PipeLineConfigInfo& configInfo);        
        ~VulkanPipeline();

        VulkanPipeline(const VulkanPipeline*) = delete;
        VulkanPipeline* operator=(const VulkanPipeline*) = delete;

        void Bind(VkCommandBuffer commandBuffer);
        void BindGlobalDescriptors(int frameIndex);
        void PushPushConstants(SimplePushConstantData push);
        void PushPushConstants(PointLightPushConstant push);

        static void DefaultPipelineConfigInfo(PipeLineConfigInfo& configInfo); 
        static void EnableAlphaBlending(PipeLineConfigInfo& congifInfo);

    private:
        static std::vector<char> readFile(const std::string& filepath);

        void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void CreatePipelineConfigInfo(VkRenderPass renderPass);

        void CreateGraphicsPipeline(
            const std::unique_ptr<VulkanShader>& shader,
            const PipeLineConfigInfo& configInfo);

        //VulkanDevice* m_VulkanDevice_;
        VkPipeline m_GraphicsPipeLine_;
        VkPipelineLayout m_PipelineLayout_;
        VkDescriptorSetLayout m_GlobalSetLayout;
        PipeLineConfigInfo m_ConfigInfo_;
        
        SystemInfo m_SystemInfo_;
        std::unique_ptr<VulkanShader> m_ShaderModule_ = nullptr;

    };
}