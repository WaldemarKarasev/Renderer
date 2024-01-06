#include "nkpch.h"
#include "UniformBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"



namespace NK {
    UniformBuffer* UniformBuffer::Create(uint32_t size, uint32_t binding)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None: NK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL: return new OpenGLUniformBuffer(size, binding);
        }
        NK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}