#include "Pipeline.hpp"

#include "nkpch.h"

#include "RendererAPI.hpp"
#include "Platform/Vulkan/VulkanPipeline.hpp"

namespace NK {

	Pipeline* Pipeline::Create(Shader* shader)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: NK_CORE_ASSERT(false, "RendererAPINone is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;// new OpenGLPipeline();
			case RendererAPI::API::Vulkan: return new VulkanPipeline(shader);
		}
		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	

    }

}