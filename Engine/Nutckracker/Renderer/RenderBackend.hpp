#pragma once

#include "Core.h"
#include "FrameInfo.hpp"
#include "Model.hpp"
#include "RendererAPI.hpp"
#include "GraphicsContext.hpp"
#include "RenderSystem.hpp"

#include "Nutckracker/Window.hpp"

namespace NK {

	class RenderBackend
	{
	public:
		// Cherno
		virtual void Init(Window* window) = 0;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(uint32_t r, uint32_t g, uint32_t b, uint32_t alpha) = 0;
		virtual void Clear() = 0;
		virtual void EnableDepthTest() = 0;
		virtual void DisableDepthTest() = 0;
		virtual void Draw(const Model* model) = 0;

		// Me
		static RenderBackend* Create();

		virtual void BeginScene() = 0;
		virtual bool BeginFrame() = 0;
		virtual void FrameProcessing(FrameInfo& frameInfo, GlobalUbo& ubo) = 0;
		virtual void BeginRendering() = 0;
		virtual void ProcessRenderingSystems(FrameInfo& frameInfo) = 0;
		virtual void EndRendering() = 0;
		virtual void EndFrame() = 0;
		virtual void EndScene() = 0;

		// We need to be able to move renderSystem inside of the RenderBackend class 
		virtual void AddRenderSystem(RenderSystem* system) = 0;
		virtual void AddRenderSurface(const Window* renderSurface) = 0;
		virtual float GetAspectRatio() = 0;



		inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		// Can be multiple RenderSystems which using this RenderBackend, but they all should use one single Graphic API.
		// That's why s_RendererAPI_ is static variable. NO WRONG
		static RendererAPI* s_RendererAPI_;
		//RendererAPI* m_RendererAPI_;
	protected:
		Window* m_RenderSurface_ = nullptr;
		std::vector<ObjectRenderSystem*> m_ObjectRenderSystems_;
		std::vector<LightRenderSystem*> m_LightRenderSystems_;

	};

}