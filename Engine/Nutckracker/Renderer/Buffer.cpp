#include "nkpch.h"
#include "Model.hpp"

#include "Renderer.hpp"

//#include "Platform/OpenGL/OpenGLBuffer.hpp"
#include "Platform/Vulkan/VulkanBuffer.hpp"


namespace NK {

    //template <typename T, typename... Rest>
    //void hashCombine(std::size_t& seed, const T& v, const Rest&... rest)
    //{
    //    //seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 9) + (seed >> 2);
    //    (hashCombine(seed, rest), ...);
    //};
}

namespace std
{
    //template<>
    //struct hash<NK::Vertex>
    //{
    //    size_t operator()(NK::Vertex const& vertex) const
    //    {
    //        size_t seed = 0;
    //        NK::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
    //        return seed;
    //    }
    //};
}

namespace NK {

	Buffer* Buffer::Create(const Builder& builder)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   NK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;//new OpenGLVertexBuffer(builder);
			case RendererAPI::API::Vulkan: return nullptr;//new VulkanBuffer(builder);
		}

		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


	std::vector<VkVertexInputBindingDescription> Vertex::GetVulkanBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;

	}

	std::vector<VkVertexInputAttributeDescription> Vertex::GetVulkanAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

        attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
        attributeDescriptions.push_back({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
        attributeDescriptions.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)});
        attributeDescriptions.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)});

        return attributeDescriptions;

	}


	void Builder::buildModel(float* vertices, uint32_t v_size, uint32_t* indices, uint32_t i_size)
	{
		NK_CORE_ASSERT(v_size % sizeof(Vertex) == 0, "Data doesn't alligned with Vertex layout!");

		m_Vertices_.clear();
		m_Indices_.clear();
		
		NK_CORE_INFO("=======TRACING VERTEX CREATION=======");
		NK_CORE_TRACE("=============== START ===============");
		for (size_t i = 0; i < v_size / sizeof(Vertex); ++i)
		{
			Vertex vertex;
			size_t offset = sizeof(Vertex)/sizeof(float) * i;
			vertex.position = glm::vec3(vertices[offset], vertices[offset + 1], vertices[offset + 2]);
			vertex.color    = glm::vec3(vertices[offset + 3], vertices[offset + 4], vertices[offset + 5]);
			vertex.normal   = glm::vec3(vertices[offset + 6], vertices[offset + 7], vertices[offset + 8]);
			vertex.uv 		= glm::vec2(vertices[offset + 9], vertices[offset + 10]);
			NK_CORE_TRACE("vertices[i + 9] = {0}, vertices[i + 10] = {1}", vertices[i + 9], vertices[i + 10]);
			if (i == 0 || i == 1 || i == 2 && 0)
			{
				NK_CORE_INFO("=======TRACING VERTEX CREATION=======");
				NK_CORE_TRACE("=============== START ===============");
				NK_CORE_TRACE("vertex.position = {0}, {1}, {2}", vertex.position.x, vertex.position.y, vertex.position.z);
				NK_CORE_TRACE("vertex.color = {0}, {1}, {2}", vertex.color.x, vertex.color.y, vertex.color.z);
				NK_CORE_TRACE("vertex.normal = {0}, {1}, {2}", vertex.normal.x, vertex.normal.y, vertex.normal.z);
				NK_CORE_TRACE("vertex.uv = {0}, {1}", vertex.position.x, vertex.position.y);
				NK_CORE_TRACE("================ END ================");
			}

			m_Vertices_.push_back(vertex);
		}
		NK_CORE_TRACE("================ END ================");

		if (i_size == 0)
		{
			NK_CORE_INFO("Indices buffer size is 0!");
			return;			
		}
		else
		{
			for (size_t i = 0; i < i_size; ++i)
			{
				m_Indices_.push_back(indices[i]);
			}
		}
	}

}