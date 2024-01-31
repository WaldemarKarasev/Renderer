#include "nkpch.h"
#include "Shader.hpp"

#include "Renderer.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Platform/Vulkan/VulkanShader.hpp"

namespace NK {

	// Shader class 
	Shader* Shader::Create(const std::string& filepath)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: NK_CORE_ASSERT(false, "RendererAPINone is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return new OpenGLShader(filepath);
			case RendererAPI::API::Vulkan: return new VulkanShader(filepath);
		}
		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Shader* Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: NK_CORE_ASSERT(false, "RendererAPINone is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return new OpenGLShader(name, vertexSrc, fragmentSrc);	
			case RendererAPI::API::Vulkan: return new VulkanShader(name, vertexSrc, fragmentSrc);
		}
		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	// Shader Library class
		void ShaderLibrary::Add(const std::string& name, Shader* shader)
		{
			NK_CORE_ASSERT(!Exists(name), "Shader already exists!");
			m_Shaders_[name] = shader;
		}

		void ShaderLibrary::Add(Shader* shader)
		{
			auto& name = shader->GetName();
			Add(name, shader);
		}

		Shader* ShaderLibrary::Load(const std::string& filepath)
		{
			auto shader = Shader::Create(filepath);
			Add(shader);
			return shader;
		}

		Shader* ShaderLibrary::Load(const std::string name, const std::string& filepath)
		{
			auto shader = Shader::Create(filepath);
			Add(name, shader);
			return shader;
		}

		Shader* ShaderLibrary::Get(const std::string& name)
		{
			NK_CORE_ASSERT(Exists(name), "Shader not found!");
			return m_Shaders_[name];
		}

		bool ShaderLibrary::Exists(const std::string& name) const
		{
			return m_Shaders_.find(name) != m_Shaders_.end();
		}
	
}