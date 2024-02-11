#include "PointLightSystem.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZEOR_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <map>
#include <stdexcept>
#include <iostream>

namespace NK
{    
    PointLightSystem::PointLightSystem(Shader* shader)
    {
        m_SystemInfo_.isLightSystem = true;
        m_Pipeline_.reset(new VulkanPipeline(shader));
    }

    void PointLightSystem::Init()
    {
        m_Pipeline_->Init(m_SystemInfo_);
    }

    PointLightSystem::~PointLightSystem()
    {
        // We only need to delete m_Pipeline, but it's a unique_ptr. It will delete itself.
        //vkDestroyPipelineLayout(m_Device_->device(), m_pipelineLayout_, nullptr);
    }

    void PointLightSystem::Update(FrameInfo& frameInfo, GlobalUbo& ubo) const
    {
        auto rotateLight = glm::rotate(glm::mat4(1.f), frameInfo.frameTime, {0.f, -1.f, 0.f});
        int lightIndex = 0;
        for(auto& kv : frameInfo.gameObjects)
        {
            auto& obj = kv.second;
            if(obj.m_PointLight_ == nullptr) continue;

            assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

            // update light position
            obj.m_transform_.translation = glm::vec3(rotateLight * glm::vec4(obj.m_transform_.translation, 1.f));

            // copy light to ubo
            ubo.pointLight[lightIndex].position = glm::vec4(obj.m_transform_.translation, 1.f);
            ubo.pointLight[lightIndex].color = glm::vec4(obj.m_color_, obj.m_PointLight_->lightIntensity);
            
            lightIndex += 1;
        }
        ubo.numLights = lightIndex;
    }

    void PointLightSystem::Render(FrameInfo& frameInfo) const
    {
        // sort light
        std::map<float, GameObject::id_t> sorted;
        for (auto& kv : frameInfo.gameObjects)
        {
            auto& obj = kv.second;
            if(obj.m_PointLight_ == nullptr) continue;

            // calculate distance
            //auto offset = frameInfo.camera.GetPosition() - obj.m_transform_.translation;
            //float disSquared = glm::dot(offset, offset);
            //sorted[disSquared] = obj.GetId();
        }
        

        m_Pipeline_->Bind();
        m_Pipeline_->BindGlobalDescriptors(frameInfo.frameIndex);

        #if 0
        vkCmdBindDescriptorSets(
            frameInfo.commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_pipelineLayout_,
            0, 
            1,
            &frameInfo.globalDescriptorSet, 
            0,
            nullptr
        );
        #endif

        // iterate through sorted light in reverse order
        for(auto it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            // use game obj id to find light object
            auto& obj = frameInfo.gameObjects.at(it->second);
            //if(obj.m_pointLight_ == nullptr) continue;

            PointLightPushConstant push{};
            push.position = glm::vec4(obj.m_transform_.translation, 1.f);
            push.coolor = glm::vec4(obj.m_color_, obj.m_PointLight_->lightIntensity);
            push.radius = obj.m_transform_.scale.x;

            m_Pipeline_->PushPushConstants(push);
            #if 0
            vkCmdPushConstants(
                frameInfo.commandBuffer,
                m_pipelineLayout_,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(PointLightPushConstant),
                &push
            );
            #endif
            
            // TODO: hide this Vulkan function inside ??? class
            vkCmdDraw(VulkanRenderBackend::s_vkCurrentFrameContext_->CurrentCommandBuffer, 6, 1, 0, 0);
        } 
    }
} 