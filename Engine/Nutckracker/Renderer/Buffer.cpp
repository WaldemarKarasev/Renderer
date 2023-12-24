#include "nkpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace NK {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size, const BufferUsage usage)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    NK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLVertexBuffer(vertices, size, usage);
		}

		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size, const BufferUsage usage)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    NK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLIndexBuffer(indices, size, usage);
		}

		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}