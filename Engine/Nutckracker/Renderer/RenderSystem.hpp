#pragma once

#include "Pipeline.hpp"
#include "FrameInfo.hpp"
#include "RendererAPI.hpp"

//#include "Platform/OpenGL/OpenGLLightRenderSystem.hpp"
//#include "Platform/OpenGL/OpenGLObjectRenderSystem.hpp"
#include "Platform/Vulkan/RenderSystems/PointLightSystem.hpp"
#include "Platform/Vulkan/RenderSystems/SimpleRenderSystem.hpp"

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



    LightRenderSystem* LightRenderSystem::CreateLightSystem(Shader* shader)
    {
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   NK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;//new OpenGLLightSystem();
			case RendererAPI::API::Vulkan: return new PointLightSystem(shader);//new VulkanLightSystem();
		}

		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }

    ObjectRenderSystem* ObjectRenderSystem::CreateObjectRenderSystem(Shader* shader)
    {
        switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   NK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;//new OpenGLLightRenderSystem();
			case RendererAPI::API::Vulkan: return new SimpleRenderSystem(shader);//new VulkanObjectRenderSystem();
		}

		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }
}