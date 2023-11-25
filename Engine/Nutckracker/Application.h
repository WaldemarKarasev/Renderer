#pragma once

#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Nutckracker/Events/Event.h"
#include "Nutckracker/LayerStack.h"
#include "Window.h"



namespace NK {

	class NK_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get()  { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window_; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window_;
		bool m_Running_ = true;
		LayerStack m_LayerStack_;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}