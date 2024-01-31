#pragma once

#include "Core.h"

#include "RenderBackend.hpp"
#include "RenderSystem.hpp"

#include "FrameInfo.hpp"
#include "Model.hpp"
#include "Nutckracker/Window.hpp"

namespace NK {

	class NK_API Renderer
	{
	public:
		void Init(Window* window);
		void BeginScene();
		bool BeginFrame();
		void FrameProcessing(FrameInfo& frameInfo, GlobalUbo& ubo);
		void BeginRendering();
		void ProcessRenderingSystems(FrameInfo& frameInfo);
		void EndRendering();		
		void EndFrame();
		void EndScene();
		void Draw(const Model* vertexArray);
		void SetViewPort(int x, int y, int width, int height);
		void AddRenderSystem(RenderSystem* renderSystem);
		float GetAspectRatio();
		RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		RenderBackend* m_RenderBackend_ = nullptr;
	};


}