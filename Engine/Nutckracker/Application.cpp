#include "nkpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace NK {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}


	Application::~Application()
	{
	}

	void Application::Run()
	{
		#if 0
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			NK_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			NK_TRACE(e);
		}
		
		size_t counter = 0;
		while (true)
		{
			counter++;
			if(counter % 10'000'000'000 == 0) {
				NK_TRACE("Hello");
			}
		};
		#endif

		while (m_Running) 
		{
			m_Window->OnUpdate();

		}
	}

}