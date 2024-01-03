#include "OpenGLShader.h"
#include "nkpch.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace NK {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		if (type == "fragment" || type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}

		NK_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);	
	}

    OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID_);
	}

	OpenGLShader::OpenGLShader(OpenGLShader&& shader) noexcept
	{
		m_RendererID_ = shader.m_RendererID_;
		shader.m_RendererID_ = 0;
	}

	OpenGLShader& OpenGLShader::operator=(OpenGLShader&& shader) noexcept
	{
		m_RendererID_ = shader.m_RendererID_;
		shader.m_RendererID_ = 0;

		return *this;

	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::string tmp = filepath;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			NK_CORE_ERROR("Could not open file '{0}'", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLenght = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while(pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			NK_CORE_ASSERT(eol != std::string::npos, "Syntax error!");
			size_t begin = pos + typeTokenLenght + 1;
			std::string type = source.substr(begin, eol - begin);
			NK_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\n\r", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string> shaderSources)
	{
		GLuint program = glCreateProgram();
		std::vector<GLenum> glShaderIDs(shaderSources.size());
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if(isCompiled == GL_FALSE)
			{
				GLint maxLenght = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLenght);

				std::vector<GLchar> infolog(maxLenght);
				glGetShaderInfoLog(shader, maxLenght, &maxLenght, &infolog[0]);

				glDeleteShader(shader);

				NK_CORE_ERROR("{0}", infolog.data());
				NK_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}
			
			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		m_RendererID_ = program;

		// Link our program.
		glLinkProgram(program);

		// Note the different functions here: glGetProgram instead of glGetShader.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLenght = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLenght);

			// The maxLenght includes the NULL character
			std::vector<GLchar> infoLog(maxLenght);
			glGetProgramInfoLog(program, maxLenght, &maxLenght, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			NK_CORE_ERROR("{0}", infoLog[0]);
			NK_CORE_ASSERT(false, "Shader link failure!");
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
		}
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID_);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}


	void OpenGLShader::SetMat4(const char* name, const glm::mat4& mat4) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID_, name), 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void OpenGLShader::SetInt(const char* name, const int value) const
	{
		glUniform1i(glGetUniformLocation(m_RendererID_, name), value);
	}

	void OpenGLShader::SetFloat(const char* name, const float value) const
	{
		glUniform1f(glGetUniformLocation(m_RendererID_, name), value);
	}
	void OpenGLShader::SetVec3(const char* name, const glm::vec3 value) const
	{
		glUniform3f(glGetUniformLocation(m_RendererID_, name), value.x, value.y, value.z);
	}
}