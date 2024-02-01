#pragma once

#include "Pipeline.hpp"
#include "FrameInfo.hpp"
#include "RendererAPI.hpp"

namespace NK {

    class RenderSystem
    {
    public:
        virtual ~RenderSystem() {
            
        }

        virtual void Init() = 0;

        //SystemInfo& GetSystemInfo() { return m_SystemInfo_; }
        bool IsLightSystem() const { return m_SystemInfo_.isLightSystem; }

    protected:
        //std::unique_ptr<Pipeline> m_Pipeline_ = nullptr;
        SystemInfo m_SystemInfo_;
    };

    class ObjectRenderSystem : public RenderSystem
    {
    public:
        virtual void Render(FrameInfo& frameInfo) const = 0;
        static ObjectRenderSystem* CreateObjectRenderSystem(Shader* shader);
    };

    class LightRenderSystem : public RenderSystem
    {
    public:
        static LightRenderSystem* CreateLightSystem(Shader* shader);
        virtual void Update(FrameInfo& frameInfo, GlobalUbo& ubo) const = 0;
        virtual void Render(FrameInfo& frameInfo) const = 0;
    };
}