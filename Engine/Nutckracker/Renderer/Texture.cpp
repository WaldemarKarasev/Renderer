#include "nkpch.h"

#include "Texture.hpp"

#include "RendererAPI.hpp"

//#include "Platform/OpenGL/OpenGLTexture.hpp"
//#include "Platform/Vulkan/VulkanTexture.hpp"

namespace NK {

    Texture2D* Texture2D::Create(const TextureSpecification& specification)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   NK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;// new OpenGLTexture2D(specification);
			case RendererAPI::API::Vulkan: return nullptr;// new VulkanTexture2D(specification);
		}

		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Texture2D* Texture2D::Create(const std::string& filepath)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   NK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;// new OpenGLTexture2D(filepath);
			case RendererAPI::API::Vulkan: return nullptr;// new VulkanTexture2D(filepath);
		}

		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}