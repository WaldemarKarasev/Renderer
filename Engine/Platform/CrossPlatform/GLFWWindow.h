#pragma once

#include "Nutckracker/Window.h"
#include "Nutckracker/Renderer/GraphicsContext.h"


//#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace NK {

	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(const WindowProps& props);
		virtual ~GLFWWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data_.Width; }
		inline unsigned int GetHeight() const override { return m_Data_.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data_.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return m_Window_; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	
	private:
		GLFWwindow* m_Window_;
		GraphicsContext* m_Context_;


		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data_;
	};

}