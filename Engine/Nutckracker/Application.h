#pragma once

#include "Core.h"

#include "Events/ApplicationEvent.h"
#include "Nutckracker/Events/Event.h"

#include "Nutckracker/ImGui/ImGuiLayer.h"
#include "Nutckracker/LayerStack.h"

#include "Nutckracker/Renderer/Camera.h"
#include "Nutckracker/Renderer/Buffer.h"
#include "Nutckracker/Renderer/Shader.h"
#include "Nutckracker/Renderer/VertexArray.h"

#include "glm/vec4.hpp"


#include "Window.h"




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

		inline Window& GetWindow() { return *m_Window_; }

		// Testing object manipulations at runtime
		//backgound colors
		inline glm::vec4& GetBackgroundColor() { return m_BackgroundColor_; };
		//model matrix
		inline float* GetScale() 		{ return scale; };
		inline float* GetRotation() 	{ return &rotate; };
		inline float* GetTranslation() 	{ return translate; };
		// view and projection matrices
		inline Camera* GetCamera() {return &camera;}
		inline float* GetCameraPosition() { return camera_position; }
		inline float* GetCameraRotation() { return camera_rotation; }
		inline bool* GetCameraMode() 	  { return &perspective_camera; }
	private:
		// Events handlers
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window_;
		ImGuiLayer* m_ImGuiLayer_;
		bool m_Running_ = true;
		LayerStack m_LayerStack_;

		std::shared_ptr<Shader> m_Shader_;
		std::shared_ptr<VertexArray> m_VertexArray_;

		std::shared_ptr<Shader> m_BlueShader_;
		std::shared_ptr<VertexArray> m_SquareVA_;
		
	private:
		// Temporaty members for testing simple transformation of rendered objects and gui manipulations at runtime

		// Changing background colors
		glm::vec4 m_BackgroundColor_ = {0.2f, 0.2f, 0.2f, 0.f};

		// Model matrix
		float scale[3] = {1.0f, 1.0f, 1.0f};
		float rotate = 0.0f;
		float translate[3] = {0.0f, 0.0f, 0.0f};

		// View and Projection matrices manipulation
		float camera_position[3] = { 0.f, 0.f, 1.f };
    	float camera_rotation[3] = { 0.f, 0.f, 0.f };
    	bool perspective_camera = false;
    	Camera camera{glm::vec3(-5., 0., 0.)};

	private:
		static Application* s_Instance_;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}