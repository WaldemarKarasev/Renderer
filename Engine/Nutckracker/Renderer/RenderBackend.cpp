#include "nkpch.h"

#include "RenderBackend.hpp"
//#include "Platform/OpenGL/OpenGLRenderBackend.hpp"
#include "Platform/Vulkan/VulkanRenderBackend.hpp"

namespace NK {

	RendererAPI* RenderBackend::s_RendererAPI_ = new RendererAPI();
	
	RenderBackend* RenderBackend::Create() {
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: 
				NK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
				return nullptr;
			case RendererAPI::API::OpenGL: 
				NK_CORE_TRACE("OpenGLRenderBackend is selected.");
				return nullptr;//new OpenGLRen();
			case RendererAPI::API::Vulkan: 
				NK_CORE_TRACE("VulkanRenderBackend is selected.");
				return new VulkanRenderBackend();
		}
		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}



}