#include "RenderSystem.hpp"

//#include "Platform/OpenGL/OpenGLLightRenderSystem.hpp"
//#include "Platform/OpenGL/OpenGLObjectRenderSystem.hpp"
#include "Platform/Vulkan/RenderSystems/PointLightSystem.hpp"
#include "Platform/Vulkan/RenderSystems/SimpleRenderSystem.hpp"


namespace NK {


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