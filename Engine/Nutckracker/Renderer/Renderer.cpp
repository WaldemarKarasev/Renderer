#include "nkpch.h"
#include "Renderer.hpp"

namespace NK {

	void Renderer::Init(Window* window)
	{
		m_RenderBackend_ = RenderBackend::Create();
		m_RenderBackend_->Init(window);
	}

	void Renderer::BeginScene()
	{
		m_RenderBackend_->BeginScene();
	}

	bool Renderer::BeginFrame()
	{
		return m_RenderBackend_->BeginFrame();
	}

	

	void Renderer::FrameProcessing(FrameInfo& frameInfo, GlobalUbo& ubo)
	{
		m_RenderBackend_->FrameProcessing(frameInfo, ubo);	
	}

	void Renderer::BeginRendering()
	{
		m_RenderBackend_->BeginRendering();
	}	

	void Renderer::ProcessRenderingSystems(FrameInfo& frameInfo)
	{
		m_RenderBackend_->ProcessRenderingSystems(frameInfo);
	}

	void Renderer::EndRendering()
	{
		m_RenderBackend_->EndRendering();
	}

	void Renderer::EndFrame()
	{
		m_RenderBackend_->EndFrame();
	}

	void Renderer::EndScene()
	{
		m_RenderBackend_->EndScene();
	}

	void Renderer::Draw(const Model* model)
	{
		m_RenderBackend_->Draw(model);
	}

	void Renderer::SetViewPort(int x, int y, int width, int height)
	{
		m_RenderBackend_->SetViewPort(x, y, width, height);
	}

	void Renderer::AddRenderSystem(RenderSystem* renderSystem)
	{
		m_RenderBackend_->AddRenderSystem(renderSystem);
	}

	float Renderer::GetAspectRatio()
	{
		float return_value = m_RenderBackend_->GetAspectRatio();
		return return_value;
	}


}