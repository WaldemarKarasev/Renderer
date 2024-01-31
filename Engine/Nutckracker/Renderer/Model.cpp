#include "nkpch.h"
#include "Model.hpp"

#include "RendererAPI.hpp"

//#include "Platform/OpenGL/OpenGLModel.hpp"
#include "Platform/Vulkan/VulkanModel.hpp"

namespace NK {

	Model* Model::Create(const Builder& builder)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   NK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;//new OpenGLModel(builder);
			case RendererAPI::API::Vulkan: return new VulkanModel(builder);
		}

		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}