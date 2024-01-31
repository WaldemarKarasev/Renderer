#include "nkpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace NK {

	static constexpr GLenum GetOpenGLBufferUsage(const BufferUsage usage)
	{
		switch (usage)
		{
		case BufferUsage::STATIC_USAGE: 	return GL_STATIC_DRAW;
		case BufferUsage::DYNAMIC_USAGE:	return GL_DYNAMIC_DRAW;
		case BufferUsage::STREAM_USAGE:		return GL_STREAM_DRAW;

		default:
			return GL_STATIC_DRAW;
		}

		NK_CORE_ERROR("Unkown BufferUsage type!");
	}

	/////////////////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, const BufferUsage usage)
	{

		glCreateBuffers(1, &m_RendererID_);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID_);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GetOpenGLBufferUsage(usage));
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID_);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID_);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/////////////////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count, const BufferUsage usage)
		: m_IndexCount_(count)
	{
		glCreateBuffers(1, &m_RendererID_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GetOpenGLBufferUsage(usage));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID_);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID_);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}