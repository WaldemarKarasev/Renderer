#include "nkpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace NK {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case NK::ShaderDataType::Float:    return GL_FLOAT;
			case NK::ShaderDataType::Float2:   return GL_FLOAT;
			case NK::ShaderDataType::Float3:   return GL_FLOAT;
			case NK::ShaderDataType::Float4:   return GL_FLOAT;
			case NK::ShaderDataType::Mat3:     return GL_FLOAT;
			case NK::ShaderDataType::Mat4:     return GL_FLOAT;
			case NK::ShaderDataType::Int:      return GL_INT;
			case NK::ShaderDataType::Int2:     return GL_INT;
			case NK::ShaderDataType::Int3:     return GL_INT;
			case NK::ShaderDataType::Int4:     return GL_INT;
			case NK::ShaderDataType::Bool:     return GL_BOOL;
		}

		NK_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID_);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID_);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID_);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		NK_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID_);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.m_Type_),
				element.m_Normalized_ ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.m_Offset_);
			index++;
		}

		m_VertexBuffers_.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID_);
		indexBuffer->Bind();

		m_IndexBuffer_ = indexBuffer;
	}

}