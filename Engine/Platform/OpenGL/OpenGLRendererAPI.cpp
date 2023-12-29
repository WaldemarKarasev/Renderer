#include "nkpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace NK {


const char* GLSourceToString(const GLenum source)
    {
        switch (source)
        {
            case GL_DEBUG_SOURCE_API: return "DEBUG_SOURCE_API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "DEBUG_SOURCE_WINDOW_SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER: return "GL_DEBUG_SOURCE_SHADER_COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY: return "DEBUG_SOURCE_THIRD_PARTY";
            case GL_DEBUG_SOURCE_APPLICATION: return "DEBUG_SOURCE_APPLICATION";
            case GL_DEBUG_SOURCE_OTHER: return "DEBUG_SOURCE_OTHER";

            default: return "UNKNOWN_DEBUG_SOURCE";
        }
    }

    const char* GLTtypeToString(const GLenum type)
    {
        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR: return "DEBUG_TYPE_ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEBUG_TYPE_DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "DEBUG_TYPE_UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY: return "DEBUG_TYPE_PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE: return "DEBUG_TYPE_PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER: return "DEBUG_TYPE_MARKER";
            case GL_DEBUG_TYPE_PUSH_GROUP: return "DEBUG_TYPE_PUSH_GROUP";
            case GL_DEBUG_TYPE_POP_GROUP: return "DEBUG_TYPE_POP_GROUP";
            case GL_DEBUG_TYPE_OTHER: return "DEBUG_TYPE_OTHER";

            default: return "UNKNOWN_DEBUG_TYPE";
        }
    }

	void OpenGLRendererAPI::Init()
	{
		// TODO: implementation
		NK_TRACE("OpenGL Contex initialized");
		//LOG_INFO("  OpenGL Vendor: {0}", get_vendor_str());
        //LOG_INFO("  OpenGL Renderer: {0}", get_renderer_str());
        //LOG_INFO("  OpenGL Version: {0}", get_version_str());

        glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		
        glDebugMessageCallback([](GLenum source,
                                  GLenum type,
                                  GLuint id,
                                  GLenum severity,
                                  GLsizei length,
                                  const GLchar* message,
                                  const void* userParam)
        {
            NK_TRACE("Debug Callback is called.");
            switch (severity)
            {
            case GL_DEBUG_SEVERITY_HIGH:
                NK_CORE_ERROR("OpenGL Error: [{0}:{1}]({2}): {3}",
                    GLSourceToString(source),
                    GLTtypeToString(type),
                    id,
                    message);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                NK_CORE_WARN("OpenGL Warning: [{0}:{1}]({2}): {3}",
                    GLSourceToString(source),
                    GLTtypeToString(type),
                    id,
                    message);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                NK_CORE_INFO("OpenGL Info: [{0}:{1}]({2}): {3}",
                    GLSourceToString(source),
                    GLTtypeToString(type),
                    id,
                    message);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                NK_CORE_TRACE("OpenGL Notificaton: [{0}:{1}]({2}): {3}",
                    GLSourceToString(source),
                    GLTtypeToString(type),
                    id,
                    message);
                break;
            default:
                NK_CORE_ERROR("OpenGL Error: [{0}:{1}] ({2}) : {3}",
                    GLSourceToString(source),
                    GLTtypeToString(type),
                    id,
                    message);
            }
        }, nullptr);
	}

	void OpenGLRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::DisableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}