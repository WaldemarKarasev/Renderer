#pragma once

#include "Core.h"

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <vulkan/vulkan.h>

namespace NK {

	enum class BufferUsage // for vertex buffers. Need to implement into vertec buffer class
	{
		STATIC_USAGE,
		DYNAMIC_USAGE,
		STREAM_USAGE
	};

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;
			case ShaderDataType::Bool:     return 1;
		}

		NK_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}




	// Will be using fixed Vertex layout. Layout will be created by builder class
    struct Vertex
	{
		glm::vec3 position{};
		glm::vec3 color{};
		glm::vec3 normal{};
		glm::vec2 uv{};
		static std::vector<VkVertexInputBindingDescription> GetVulkanBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> GetVulkanAttributeDescriptions();
		// For OpenGL
		// static BufferLayout GetBufferLayout();

		bool operator==(const Vertex& other) const
		{
			return position == other.position && color == other.color && normal == other.normal &&
					uv == other.uv;
		}
	};

	struct Builder
	{
		std::vector<Vertex> m_Vertices_{};
		std::vector<uint32_t> m_Indices_{};
		
		void buildModel(float* vertices, uint32_t v_size, uint32_t* indices, uint32_t i_size = 0);
		void loadModel(const std::string& filepath) { /*empty inplementation*/ }
	};
	
	class NK_API Buffer
	{
	public:
		virtual ~Buffer() {}

		static Buffer* Create(const Builder& builder);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;


	};
}