#include "nkpch.h"
#include "Application.hpp"

#include <glad/glad.h>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Nutckracker/Renderer/Renderer.hpp"
#include "Nutckracker/Renderer/RenderSystem.hpp"

#include "Input.h"

#include "Nutckracker/Log.h"

// for testing texture generation
#include "Utils/ApplicationTextures.hpp"
#include "Utils/VerticesAndShaders.hpp"


namespace NK {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance_ = nullptr;

	Application::Application()
	{	
		NK_CORE_ASSERT(!s_Instance_, "Applicaiton already exists!");
		s_Instance_ = this;
		
		m_Window_ = Window::Create();
		m_Window_->SetEventCallback(BIND_EVENT_FN(OnEvent));

		// TODO: vulkan implementation
		#if 0
		m_ImGuiLayer_ = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer_);	
		#endif

		m_Renderer_ = std::make_unique<Renderer>(); 
		m_Renderer_->Init(m_Window_); // Vulkan initialization

		// 1. Init game objects and fill GameObjects::Map with models a.k.a. gameObjects
		FillGameObjects();
		// 2. Init RenderSystems and add them into Renderer
		CreateRenderSystems();
	}

	void Application::CreateRenderSystems()
	{
		// Creating Shaders and Pipeline for ObjectsRenderingSystems

		// Creting Vulkan Shader Module at this stage. That is means that at this point of the program qe already initialized Vulkan Context
		// If we want to stop at stage of compiling shaders source code to SPIR-V format without creating shader module. We need to cached out current shader state.
		// That means We need to compile shader and wait for contex creation. This possible could be usefulfor Graphics API changing at runtime. 
		Shader* ObjectShader = Shader::Create("Cube Shader", VulkanVertexShader, VulkanFragmentShader);

		// Creating Pipeline. For its initialization Pipeline is using Graphic API context, which should be already inited at this stage of proram.


		// Creating Shaders and Pipeline for LightRenderingSystem
		Shader* LightShader = Shader::Create("Light Shader", VulkanPointLightVertexShader, VulkanPointLightFragmentFragmentShader);

		LightRenderSystem* LightSystem = LightRenderSystem::CreateLightSystem(LightShader);
		ObjectRenderSystem* ObjectSystem = ObjectRenderSystem::CreateObjectRenderSystem(ObjectShader);

		m_Renderer_->AddRenderSystem(LightSystem);
		m_Renderer_->AddRenderSystem(ObjectSystem);
	}

	void Application::FillGameObjects()
	{
		// TODO: proper realization
		Builder builder;
		//builder.buildModel(...);
		//builder.buildModel(ve)
		std::shared_ptr<Model> model;
		model.reset(Model::Create(builder));

		auto cube = GameObject::CreateGameObject();
		cube.m_Model_ = model;

		m_GameObjets_.emplace(cube.GetId(), std::move(cube));
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack_.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack_.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));


		for (auto it = m_LayerStack_.end(); it != m_LayerStack_.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{	
		auto currentTime = std::chrono::high_resolution_clock::now();
		while (m_Running_) 
		{
			auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

			// Updating m_Camera_

			m_Camera_.SetViewYXZ(m_Camera_.GetCameraTranslation(), m_Camera_.GetCameraRotation());

			m_Camera_.SetPerspectiveProjection(glm::radians(50.f), m_Renderer_->GetAspectRatio(), 0.1f, 100.f);


			m_Renderer_->BeginScene();
			if (m_Renderer_->BeginFrame())
			{
				int frameIndex;
				FrameInfo frameInfo{
					frameIndex,
					frameTime,
					m_Camera_, // already updated
					m_GameObjets_
				};

				GlobalUbo ubo{};
				ubo.projection = m_Camera_.GetProjectionMatrix();
				ubo.view = m_Camera_.GetViewMatrix();
				ubo.inverseView = m_Camera_.GetInverseViewMatrix();

				m_Renderer_->FrameProcessing(frameInfo, ubo);
				m_Renderer_->BeginRendering();
				m_Renderer_->ProcessRenderingSystems(frameInfo);
				m_Renderer_->EndRendering();
				m_Renderer_->EndFrame();
			}
			m_Renderer_->EndScene();

			#if 0
			for (Layer* layer : m_LayerStack_)
				layer->OnUpdate();

			m_ImGuiLayer_->Begin();
			for (Layer* layer : m_LayerStack_)
				layer->OnImGuiRender();
			m_ImGuiLayer_->End();
			#endif

			m_Window_->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		NK_CORE_TRACE("Window closed callback");
		m_Running_ = false;
		return true;
	}

	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		//NK_CORE_TRACE("Window resized callback");
		//m_Renderer_->SetViewPort(0, 0, e.GetWidth(), e.GetHeight());
		//camera.SetViewPortSize(e.GetWidth(), e.GetHeight());
		return true;
	}
}