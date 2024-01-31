#pragma once

#include "Nutckracker/Window.hpp"


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

		virtual void CreateVKSurface(VkInstance instance, VkSurfaceKHR* surface) const override;


		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data_.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return m_Window_; }

		// Me
		virtual const char** GetRequiredExtensions(uint32_t* extensionsCount) const override { return glfwGetRequiredInstanceExtensions(extensionsCount); }
		virtual void WaitEvents() const override { glfwWaitEvents(); }

	private:

		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	
	private:
		GLFWwindow* m_Window_;
		
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