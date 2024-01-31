#include "nkpch.h"
#include "UniformBuffer.h"

#include "RendererAPI.hpp"
//#include "Platform/OpenGL/OpenGLUniformBuffer.h"
//#include "Platform/Vulkan/VulkanUniformBuffer.h"


namespace NK {
    UniformBuffer* UniformBuffer::Create(uint32_t size, uint32_t binding)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::None: NK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return nullptr;//new OpenGLUniformBuffer(size, binding);
            case RendererAPI::API::Vulkan: return nullptr;//new VulkanUniformBuffer(size, binding);
        }
        NK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}