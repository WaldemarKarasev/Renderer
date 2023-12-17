#pragma once

#include "Nutckracker/Renderer/Buffer.h"

namespace NK {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_layout;}
		virtual void SetLayout(const BufferLayout& layout) override { m_layout = layout; }

	private:
		uint32_t m_RendererID;
		BufferLayout m_layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return m_Count_; }
	private:
		uint32_t m_RendererID_;
		uint32_t m_Count_;
	};

}