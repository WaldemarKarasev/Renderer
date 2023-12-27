#pragma once

#include "Core.h"

#include "RendererAPI.h"

namespace NK_API NK {

	class RenderCommand
	{
	public:

		static void Init()
		{
			s_RendererAPI_->Init();
		}

		static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI_->SetViewPort(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI_->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI_->Clear();
		}

		inline static void EnableDepthTest()
		{
			s_RendererAPI_->EnableDepthTest();
		}

		inline static void DisableDepthTest()
		{
			s_RendererAPI_->DisableDepthTest();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI_->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_RendererAPI_;
	};

}