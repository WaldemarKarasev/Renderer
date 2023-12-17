#pragma once

#include "Core.h"

#include "Events/ApplicationEvent.h"

#include "Nutckracker/Events/Event.h"

#include "Nutckracker/ImGui/ImGuiLayer.h"
#include "Nutckracker/LayerStack.h"

#include "Nutckracker/Renderer/Shader.h"
#include "Nutckracker/Renderer/Buffer.h"
#include "Nutckracker/Renderer/VertexArray.h"


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
	
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window_;
		ImGuiLayer* m_ImGuiLayer_;
		bool m_Running_ = true;
		LayerStack m_LayerStack_;

		std::shared_ptr<Shader> m_Shader_;
		std::shared_ptr<VertexArray> m_VertexArray_;

		std::shared_ptr<Shader> m_BlueShader_;
		std::shared_ptr<VertexArray> m_SquareVA_;
		
	private:
		static Application* s_Instance_;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}