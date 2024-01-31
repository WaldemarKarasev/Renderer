#pragma once

#include "Core.h"

#include "Nutckracker/Renderer/GraphicsContext.hpp"

struct NK_API GLFWwindow;

namespace NK {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		
	private:
		GLFWwindow* m_WindowHandle_;
	};

}