#include "nkpch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace NK {

	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			NK_CORE_ERROR("{0}", infoLog.data());
			NK_CORE_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			NK_CORE_ERROR("{0}", infoLog.data());
			NK_CORE_ASSERT(false, "Fragment shader compilation failure!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID_ = glCreateProgram();
		GLuint program = m_RendererID_;

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

		    NK_CORE_ERROR("{0}", infoLog.data());
		    NK_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID_);
	}

	Shader::Shader(Shader&& shader)
	{
		m_RendererID_ = shader.m_RendererID_;
		m_IsCompiled_ = shader.m_IsCompiled_;

		shader.m_RendererID_ = 0;
		shader.m_IsCompiled_ = false;
	}

	Shader& Shader::operator=(Shader&& shader)
	{
		m_RendererID_ = shader.m_RendererID_;
		m_IsCompiled_ = shader.m_IsCompiled_;

		shader.m_RendererID_ = 0;
		shader.m_IsCompiled_ = false;

		return *this;

	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID_);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}


	void Shader::SetMat4(const char* name, const glm::mat4& mat4) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID_, name), 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void Shader::SetInt(const char* name, const int value) const
	{
		glUniform1i(glGetUniformLocation(m_RendererID_, name), value);
	}

	void Shader::SetFloat(const char* name, const float value) const
	{
		glUniform1f(glGetUniformLocation(m_RendererID_, name), value);
	}
	void Shader::SetVec3(const char* name, const glm::vec3 value) const
	{
		glUniform3f(glGetUniformLocation(m_RendererID_, name), value.x, value.y, value.z);
	}
}