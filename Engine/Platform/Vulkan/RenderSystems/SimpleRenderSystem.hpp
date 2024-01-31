#pragma once 
// std
#include <memory>
#include <vector>

#include "Nutckracker/Renderer/RenderSystem.hpp"

#include "../VulkanDevice.hpp"
#include "../VulkanPipeline.hpp"
#include "../VulkanRenderBackend.hpp"
#include "../VulkanSwapChain.hpp"



namespace NK
{
    class SimpleRenderSystem : public ObjectRenderSystem
    {
    public:
        // Me. ObjectRenderSystem Interface
        SimpleRenderSystem(Shader* shader);
        virtual void Init() override;
        virtual void Render(FrameInfo& frameInfo) const override;
        //

        //SimpleRenderSystem(VulkanDevice* device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

        void renderGameObjects(FrameInfo& frameInfo);
        
    private:
        std::unique_ptr<VulkanPipeline> m_Pipeline_;
    };
}
