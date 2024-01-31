#include "nkpch.h"
#include "GraphicsContext.hpp"
//#include "Platform/OpenGL/OpenGLContext.hpp"
//#include "Platform/Vulkan/VulkanContext.hpp"

namespace NK
{
	GraphicsContext* GraphicsContext::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: NK_CORE_ASSERT(false, "RendererAPINone is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;//new OpenGLContext();	
			case RendererAPI::API::Vulkan: return nullptr;//new VulkanContext();
		}
		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}