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
    class PointLightSystem : public LightRenderSystem
    {
    public:
        PointLightSystem(Shader* shader);

        virtual void Init() override;
        virtual void Update(FrameInfo& frameInfo, GlobalUbo& ubo) const override;
        virtual void Render(FrameInfo& frameInfo) const override;
        
        ~PointLightSystem();

        //PointLightSystem(const PointLightSystem&) = delete;
        //PointLightSystem& operator=(const PointLightSystem&) = delete;

    private:
        std::unique_ptr<VulkanPipeline> m_Pipeline_;
    };
}
