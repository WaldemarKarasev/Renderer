#include "hzpch.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace MK {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}