#include "nkpch.h"

#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace NK {

	RendererAPI* RenderCommand::s_RendererAPI_ = new OpenGLRendererAPI;

}