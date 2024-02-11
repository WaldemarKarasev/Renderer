#pragma once

#include "Core.h"

#include "Events/ApplicationEvent.h"
#include "Nutckracker/Events/Event.h"

#include "Nutckracker/ImGui/ImGuiLayer.h"
#include "Nutckracker/LayerStack.h"


#include "Nutckracker/Renderer/Buffer.hpp"
#include "Nutckracker/Renderer/Camera.hpp"
#include "Nutckracker/Renderer/GameObject.hpp"
#include "Nutckracker/Renderer/Model.hpp"
#include "Nutckracker/Renderer/Renderer.hpp"
#include "Nutckracker/Renderer/Shader.hpp"
#include "Nutckracker/Renderer/Texture.hpp"

#include "glm/vec4.hpp"


#include "Window.hpp"


class GameObject;


namespace NK {

	class NK_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get()  { return *s_Instance_; }

		inline Camera* GetCamera() { return &m_Camera_; }

		inline Window& GetWindow() { return *m_Window_; }

	private:
		// Events handlers
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		Window* m_Window_ = nullptr;
		ImGuiLayer* m_ImGuiLayer_;
		bool m_Running_ = true;
		LayerStack m_LayerStack_;

		// Camera object
    	Camera m_Camera_{glm::vec3{0.0f, 0.0f, -20.0f}};

		// Renderer
		// Renderer class is independent from API's (Vulkan or OpenGL)
		std::unique_ptr<Renderer> m_Renderer_;
		void CreateRenderSystems();

	private:
		// Game objects
		GameObject::Map m_GameObjets_;
		void FillGameObjects();
	#if 0
		std::shared_ptr<Shader> m_BlueShader_;
		std::shared_ptr<Model> m_SquareVA_;
	
		Texture2D* m_SmileTexture_ = nullptr;
		std::shared_ptr<Shader> m_LightSourceShader_;
		std::shared_ptr<Model> m_LightSourceIceCube_;
	#endif
	private:
		static Application* s_Instance_;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}