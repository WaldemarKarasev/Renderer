#pragma once

#include "Core.h"

#include "RendererAPI.h"

namespace NK_API NK {

	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI_->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI_->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI_->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_RendererAPI_;
	};

}