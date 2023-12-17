#pragma once

#include "Nutckracker/Renderer/VertexArray.h"

namespace NK {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers_; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer_; }
	private:
		uint32_t m_RendererID_;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers_;
		std::shared_ptr<IndexBuffer> m_IndexBuffer_;
	};

}