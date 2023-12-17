#include "nkpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace NK {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle_(windowHandle)
	{
		NK_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle_);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		NK_CORE_ASSERT(status, "Failed to initialize Glad!");	

		NK_CORE_INFO("OpenGL Info:");
		//NK_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		//NK_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		//NK_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle_);
	}

}