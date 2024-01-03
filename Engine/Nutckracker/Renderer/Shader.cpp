#include "nkpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace NK {

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: NK_CORE_ASSERT(false, "RendererAPINone is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLShader(vertexSrc, fragmentSrc);	
		}
		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	
}