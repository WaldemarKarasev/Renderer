#include "SimpleRenderSystem.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZEOR_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <stdexcept>
#include <iostream>
namespace NK
{    
    SimpleRenderSystem::SimpleRenderSystem(Shader* shader)
    {
        m_SystemInfo_.isLightSystem = false;
        m_Pipeline_.reset(new VulkanPipeline(shader));
    }

    void SimpleRenderSystem::Init()
    {
        m_Pipeline_->Init(m_SystemInfo_);
    }

    #if 0
    SimpleRenderSystem::SimpleRenderSystem(VulkanDevice* device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
    : m_Device_{device}
    {
        createPipelineLayout(globalSetLayout);
        // compare if renderpass is compatable
        createPipeline(renderPass);
    }
    #endif

    SimpleRenderSystem::~SimpleRenderSystem()
    {
        // We only need to delete m_Pipeline, but it's a unique_ptr. It will delete itself.
        //vkDestroyPipelineLayout(m_Device_->device(), m_pipelineLayout_, nullptr);
    }

    void SimpleRenderSystem::renderGameObjects(FrameInfo& frameInfo)
    {
        //m_Pipeline_->bind(frameInfo.commandBuffer);
        m_Pipeline_->Bind();
        m_Pipeline_->BindGlobalDescriptors(frameInfo.frameIndex);
        #if 0
        vkCmdBindDescriptorSets(
            VulkanRenderBackend::s_vkCurrentFrameContext_->CurrentCommandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_pipelineLayout_,
            0, 
            1,
            &VulkanRenderBackend::GetVKContext()->GlobalDescriptorSet, 
            0,
            nullptr
        );
        #endif

        for (auto& kv : frameInfo.gameObjects)
        {
            //obj.m_transform_.rotation.y = glm::mod(obj.m_transform_.rotation.y + 0.01f, glm::two_pi<float>());
            //obj.m_transform_.rotation.x = glm::mod(obj.m_transform_.rotation.x + 0.01f, glm::two_pi<float>());
            auto& obj = kv.second;

            if(obj.m_Model_ == nullptr) continue;

            SimplePushConstantData push{};
            push.modelMatrix = obj.m_transform_.mat4();
            push.normalMatrix = obj.m_transform_.normalMatrix();

            m_Pipeline_->PushPushConstants(push);
            #if 0            
            vkCmdPushConstants( 
                VulkanRenderBackend::GetVKContext()->CurrentCommandBuffer, 
                m_pipelineLayout_, 
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 
                0, 
                sizeof(SimplePushConstantData), 
                &push);
            #endif

            obj.m_Model_->Bind();//obj.m_model_->bind(frameInfo.commandBuffer);
            obj.m_Model_->Draw();//obj.m_model_->draw(frameInfo.commandBuffer);
        }   
    }
} 