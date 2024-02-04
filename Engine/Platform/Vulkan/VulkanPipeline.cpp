#include "VulkanPipeline.hpp"
#include "VKContext.hpp"

// std
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cassert>

#ifndef ENGINE_DIR
#define ENGINE_DIR "../"
#endif

namespace NK
{
    VulkanPipeline::~VulkanPipeline()
    {
        //vkDestroyShaderModule(m_VulkanDevice_->device(), m_vertShaderModule_, nullptr);
        //vkDestroyShaderModule(m_VulkanDevice_->device(), m_fragShaderModule_, nullptr);
        // 1. delete m_ShaderModule_; // No need to delete m_ShaderModule because it's unique_ptr
        // 2. m_ConfigInfo_ simple struct. Simple deletion
        // 3. 
        vkDestroyPipeline(vkContext.device, m_GraphicsPipeLine_, nullptr);
        vkDestroyPipelineLayout(vkContext.device, m_PipelineLayout_, nullptr);
    }


    //Me
    VulkanPipeline::VulkanPipeline(Shader* shader)
    {
        if (dynamic_cast<VulkanShader*>(shader) == nullptr)
        {
            NK_CORE_ASSERT(false, "Failed dynamic_cast to VulkanShader!");
        }

        // Casting to m_ShaderModule to save incoming shader inside of VulkanPipeline object
        m_ShaderModule_.reset(dynamic_cast<VulkanShader*>(shader));
    }

    VulkanPipeline::VulkanPipeline(Shader* shader, const PipeLineConfigInfo& configInfo){
        // empty. Maybe this ctor will not be needed
    }

    void VulkanPipeline::Init(SystemInfo& systemInfo)
    {
        m_SystemInfo_ = systemInfo;
        VulkanRenderBackendContext* vkRenderBackendContext = VulkanRenderBackend::s_vkRenderBackendContext_;
        VulkanRenderBackend* vkRenderBackend = VulkanRenderBackend::s_vkRenderBackend_;
        CreatePipelineLayout(vkRenderBackendContext->globalDescriptorSetLayout);
        CreatePipelineConfigInfo(vkRenderBackend->GetSwapChainRenderPass());

        CreateGraphicsPipeline(m_ShaderModule_, m_ConfigInfo_);
    }


    void VulkanPipeline::Bind() 
    {
        NK_CORE_ASSERT(VulkanRenderBackend::s_vkCurrentFrameContext_->CurrentCommandBuffer != nullptr, "Current Command Buffer is nullptr! Unable to bind pipeline!");
        Bind(VulkanRenderBackend::s_vkCurrentFrameContext_->CurrentCommandBuffer);
    }


    void VulkanPipeline::Bind(VkCommandBuffer commandBuffer)
    {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeLine_);
    }

    void VulkanPipeline::BindGlobalDescriptors(int frameIndex)
    {
        vkCmdBindDescriptorSets(
            VulkanRenderBackend::s_vkCurrentFrameContext_->CurrentCommandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_PipelineLayout_,
            0, 
            1,
            &VulkanRenderBackend::s_vkRenderBackendContext_->descriptorSets[frameIndex], 
            0,
            nullptr
        );
    }

    void VulkanPipeline::PushPushConstants(SimplePushConstantData push)
    {
        vkCmdPushConstants( 
            VulkanRenderBackend::s_vkCurrentFrameContext_->CurrentCommandBuffer, 
            m_PipelineLayout_, 
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 
            0, 
            sizeof(SimplePushConstantData), 
            &push);
    }

    void VulkanPipeline::PushPushConstants(PointLightPushConstant push)
    {
        vkCmdPushConstants( 
            VulkanRenderBackend::s_vkCurrentFrameContext_->CurrentCommandBuffer, 
            m_PipelineLayout_, 
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 
            0, 
            sizeof(PointLightPushConstant), 
            &push);
    }

    std::vector<char> VulkanPipeline::readFile(const std::string& filepath)
    {
        std::string enginePath = ENGINE_DIR + filepath;
        std::ifstream file(enginePath, std::ios::ate | std::ios::binary);
        
        if(!file.is_open())
        {
            throw std::runtime_error("failed to open file: " + filepath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> ret_buffer(fileSize);

        file.seekg(0);
        file.read(ret_buffer.data(), fileSize);

        file.close();
        return ret_buffer;
    }


    void VulkanPipeline::CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout)
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;

        if (m_SystemInfo_.isLightSystem)
        {
            pushConstantRange.size = sizeof(PointLightPushConstant);
        }
        else
        {
            pushConstantRange.size = sizeof(SimplePushConstantData);
        }

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout, globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if(vkCreatePipelineLayout(vkContext.device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout_) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void VulkanPipeline::CreatePipelineConfigInfo(VkRenderPass renderPass)
    {
        //assert(m_PipelineLayout_ != nullptr && "Cannot create pipeline before pipeline layout!");
        NK_CORE_ASSERT(m_PipelineLayout_ != nullptr, "Cannot create pipeline before pipeline layout!");
        
        if (m_SystemInfo_.isLightSystem)
        {
            VulkanPipeline::DefaultPipelineConfigInfo(m_ConfigInfo_);
            VulkanPipeline::EnableAlphaBlending(m_ConfigInfo_);
            m_ConfigInfo_.attributeDescriptions.clear();
            m_ConfigInfo_.bindingDescriptions.clear();
            m_ConfigInfo_.renderPass = renderPass;
            m_ConfigInfo_.pipelineLayout = m_PipelineLayout_;

        }   
        else
        {
            VulkanPipeline::DefaultPipelineConfigInfo(m_ConfigInfo_);
            m_ConfigInfo_.renderPass = renderPass;
            m_ConfigInfo_.pipelineLayout = m_PipelineLayout_;
        }     
    }


    void VulkanPipeline::CreateGraphicsPipeline(
        const std::unique_ptr<VulkanShader>& shader,
        const PipeLineConfigInfo& configInfo)
    {
        //assert(configInfo.pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
        //assert(configInfo.renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline: no renderPass provided in configInfo");

        NK_CORE_ASSERT(configInfo.pipelineLayout != VK_NULL_HANDLE, "Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
        NK_CORE_ASSERT(configInfo.renderPass != VK_NULL_HANDLE, "Cannot create graphics pipeline: no renderPass provided in configInfo");

        VkPipelineShaderStageCreateInfo shaderStages[2];
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = shader->GetVertexShaderModule();
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = shader->GetFragmentShaderModule();
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;

        //auto attributeDescriptions = LveModel::Vertex::getAttributeDescriptions();
        //auto bindingDescriptions = LveModel::Vertex::getBindingDescriptions();
        
        auto& attributeDescriptions = configInfo.attributeDescriptions;
        auto& bindingDescriptions = configInfo.bindingDescriptions;
        

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();        

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
        pipelineInfo.pViewportState = &configInfo.viewportInfo;
        pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
        pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
        pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
        pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
        pipelineInfo.pDynamicState = &configInfo.dynamicStateInfo;

        pipelineInfo.layout = configInfo.pipelineLayout;
        pipelineInfo.renderPass = configInfo.renderPass;
        pipelineInfo.subpass = configInfo.subpass;

        pipelineInfo.basePipelineIndex = -1;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(
                vkContext.device,
                VK_NULL_HANDLE,
                1,
                &pipelineInfo,
                nullptr,
                &m_GraphicsPipeLine_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline");
        }

    }

    #if 0
    // Vulkan Shader Modules created and stored inside of a m_ShaderModule object
    void VulkanPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if(vkCreateShaderModule(m_VulkanDevice_->device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
        { 
            throw std::runtime_error("failed to create shader module");
        }
    }
    #endif



    void VulkanPipeline::DefaultPipelineConfigInfo(PipeLineConfigInfo& configInfo)
    {

        configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        configInfo.viewportInfo.viewportCount = 1;
        configInfo.viewportInfo.pViewports = nullptr;
        configInfo.viewportInfo.scissorCount = 1;
        configInfo.viewportInfo.pScissors = nullptr;

        configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
        configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;//VK_POLYGON_MODE_LINE;
        configInfo.rasterizationInfo.lineWidth = 1.0f;
        configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
        configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
        configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
        configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
        configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

        configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
        configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
        configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
        configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
        configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

        configInfo.colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
        configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
        configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
        configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

        configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
        configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
        configInfo.colorBlendInfo.attachmentCount = 1;
        configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
        configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

        configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
        configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
        configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.front = {};  // Optional
        configInfo.depthStencilInfo.back = {};   // Optional

        configInfo.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
        configInfo.dynamicStateInfo.dynamicStateCount =
        static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
        configInfo.dynamicStateInfo.flags = 0; 

        configInfo.bindingDescriptions = NK::Vertex::GetVulkanBindingDescriptions();
        configInfo.attributeDescriptions = NK::Vertex::GetVulkanAttributeDescriptions();

    }


    void VulkanPipeline::EnableAlphaBlending(PipeLineConfigInfo& configInfo)
    {
        configInfo.colorBlendAttachment.blendEnable = VK_TRUE;

        configInfo.colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
        
        configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              
        configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   
        configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  
        configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              
    }
}