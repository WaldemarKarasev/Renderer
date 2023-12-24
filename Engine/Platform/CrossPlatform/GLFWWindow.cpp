#include "nkpch.h"
#include "GLFWWindow.h"

#include "Nutckracker/Events/ApplicationEvent.h"
#include "Nutckracker/Events/KeyEvent.h"
#include "Nutckracker/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace NK {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		NK_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new GLFWWindow(props);
	}

	GLFWWindow::GLFWWindow(const WindowProps& props)
	{
		Init(props);
	}

	GLFWWindow::~GLFWWindow()
	{
		Shutdown();
	}

	void GLFWWindow::Init(const WindowProps& props)
	{
		m_Data_.Title = props.Title;
		m_Data_.Width = props.Width;
		m_Data_.Height = props.Height;

		NK_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			NK_CORE_ASSERT(success, "Could not intialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		m_Window_ = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data_.Title.c_str(), nullptr, nullptr);
		
		m_Context_ = new OpenGLContext(m_Window_);
		m_Context_->Init();

		glfwSetWindowUserPointer(m_Window_, &m_Data_);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window_, [](GLFWwindow* window, int width, int height){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width  = width;
			data.Height = height;
			NK_TRACE("WindowSizeCallback: {0}, {1}", width, height);
			WindowResizeEvent event(width, height);
			
			data.EventCallback(event);
		});

		glfwSetFramebufferSizeCallback(m_Window_, [](GLFWwindow* window, int width, int height){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			NK_TRACE("FramebufferSizeCallback: {0}, {1}", width, height);
		});

		glfwSetWindowCloseCallback(m_Window_, [](GLFWwindow* window){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window_, [](GLFWwindow* window, int key, int scancode, int action, int mods){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window_, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window_, [](GLFWwindow* window, int button, int action, int mods){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window_, [](GLFWwindow* window, double xOffset, double yOffset){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);

		});

		glfwSetCursorPosCallback(m_Window_, [](GLFWwindow* window, double xPos, double yPos){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event(xPos, yPos);

			data.EventCallback(event);
		});

	}

	void GLFWWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window_);
	}

	void GLFWWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context_->SwapBuffers();
	}

	void GLFWWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data_.VSync = enabled;
	}

	bool GLFWWindow::IsVSync() const
	{
		return m_Data_.VSync;
	}

}