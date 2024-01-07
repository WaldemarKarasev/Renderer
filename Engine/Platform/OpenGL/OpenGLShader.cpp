#include "OpenGLShader.h"
#include "nkpch.h"

#include <fstream>
//#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>



namespace NK {

	namespace detail {
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

		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
			}
			NK_CORE_ASSERT(false, "Unknown shaderc_shader_kind type!");
			return static_cast<shaderc_shader_kind>(0);
		}
		
		static const char* GLShaderStageToString(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
			}
			NK_CORE_ASSERT(false, "Unknown shaderc_shader_kind type!");
			return nullptr;
		}

		static const char* GetCacheDirectory()
		{
			// TODO: make sure the assets directpry is valid
			return "/home/pingvinus/prog/game_eng/Renderer/ASSETS";
		}

		static void CreateCacheSirectoryIfNeeded()
		{
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::experimental::filesystem::exists(cacheDirectory))
			{
				std::experimental::filesystem::create_directories(cacheDirectory);
			}
		}

		static const char* GLShaderStageCacheOpenGLFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER: return ".cached_opengl.vert";
			case GL_FRAGMENT_SHADER: return ".cached_opengl.frag";
			}
			NK_CORE_ASSERT(false, "Unsupported stage for Cached OpenGL File Extension!");
			return "";
		}

		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER: return ".cached_vulkan.vert";
			case GL_FRAGMENT_SHADER: return ".cached_vulkan.frag";
			}
			NK_CORE_ASSERT(false, "Unsupported stage for Cached Vulkan File Extension!");
			return "";		
		}
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	: m_FilePath_(filepath)
	{
		detail::CreateCacheSirectoryIfNeeded();
		
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		
		{
			//Timer timer; //TODO: timer implementation
			CompileOrGetVulkanBinaries(shaderSources);
			CompileOrGetOpenGLBinaries();
			CreateProgram();
			//NK_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
		}

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name_ = filepath.substr(lastSlash, count);
	}

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	: m_Name_(name)
	{
		NK_TRACE("shader name: {0}", name);
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		CompileOrGetVulkanBinaries(sources);
		CompileOrGetOpenGLBinaries();
		CreateProgram();
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID_);
	}
	
	#if 0
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
	#endif

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::string tmp = filepath;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstreanm closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());
			}
			else
			{
				NK_CORE_ERROR("Could not read from file '{0}", filepath);
			}
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
		size_t pos = source.find(typeToken, 0); // Start of shader type declaration line
		while(pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); // End of shader type declaration line
			NK_CORE_ASSERT(eol != std::string::npos, "Syntax error!");
			size_t begin = pos + typeTokenLenght + 1; // Start of shader type name (after "#type" keyword)
			std::string type = source.substr(begin, eol - begin);
			NK_CORE_ASSERT(detail::ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\n\r", eol); // Start of shader code after shader type application line
			NK_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error!");
			pos = source.find(typeToken, nextLinePos); // Startof next shader type declaration line

			shaderSources[detail::ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSources;
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint proram = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		const bool optimize = true;

		if (optimize)
		{
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}

		std::experimental::filesystem::path cachedDiretory = detail::GetCacheDirectory();
		auto& shaderData = m_VulkanSPIRV_;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources)
		{
			std::experimental::filesystem::path shaderFilePath = m_FilePath_;
			std::experimental::filesystem::path cachedPath = cachedDiretory / (shaderFilePath.filename().string() + detail::GLShaderStageCachedVulkanFileExtension(stage));

			NK_CORE_TRACE("shaderFilePath - {0}", shaderFilePath.string());
			NK_CORE_TRACE("cachedPath - {0}", cachedPath.string());

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				NK_CORE_TRACE("trace if");
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				NK_CORE_TRACE("trace else, line {0}", __LINE__);
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, detail::GLShaderStageToShaderC(stage), m_FilePath_.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					NK_CORE_ERROR(module.GetErrorMessage());
					NK_CORE_ASSERT(false, "CompileOrGetVulkanBinaries: Module compilation error!");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if(out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto& [stage, data] : shaderData)
		{
			//Reflect(stage, data);
		}

	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		NK_TRACE("\n\nOpenGLShader::CompileOrGetOpenGLBinaries()");
		auto& shaderData = m_OpenGLSPIRV_;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false;
		if (optimize)
		{
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}

		std::experimental::filesystem::path cacheDirectory = detail::GetCacheDirectory();

		shaderData.clear();
		m_OpenGLSourceCode_.clear();
		for (auto&& [stage, spirv] : m_VulkanSPIRV_)
		{
			std::experimental::filesystem::path shaderFilePath = m_FilePath_;
			std::experimental::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string(), + detail::GLShaderStageCacheOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_OpenGLSourceCode_[stage] = glslCompiler.compile();
				auto& source = m_OpenGLSourceCode_[stage];

				//NK_TRACE("Ready to be compiled GLSL source code:");
				//NK_TRACE(source);

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, detail::GLShaderStageToShaderC(stage), m_FilePath_.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					NK_CORE_ERROR(module.GetErrorMessage());
					NK_CORE_ASSERT(false, "CompileOrGetOpenGLBinaries: Module compilation error!");
				}

				shaderData[stage] = std::vector<uint32_t>(module.begin(), module.end());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

	}

	void OpenGLShader::CreateProgram()
	{	
		#if 1
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_OpenGLSPIRV_)
		{
			//GLuint tmp = glCreateShader(stage);
			//GLuint shaderID = shaderIDs.emplace_back(tmp);
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH,  &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
			{
				glDeleteShader(id);
			}
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID_ = program;
		#else
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = m_OpenGLSourceCode_[GL_VERTEX_SHADER].c_str();
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
		source = m_OpenGLSourceCode_[GL_FRAGMENT_SHADER].c_str();
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
		#endif
	}

	void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		NK_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", detail::GLShaderStageToString(stage), m_FilePath_);
		NK_CORE_TRACE("		{0} uniform buffers", resources.uniform_buffers.size());
		NK_CORE_TRACE("		{0} resources", resources.sampled_images.size());

		NK_CORE_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			NK_CORE_TRACE("		{0}", resource.name);
			NK_CORE_TRACE("		Size = {0}", bufferSize);
			NK_CORE_TRACE("		Binding = {0}", binding);
			NK_CORE_TRACE("		Members = {0}", memberCount);
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

	void OpenGLShader::SetInt(const char* name, const int value) const
	{
		glUniform1i(glGetUniformLocation(m_RendererID_, name), value);
	}

	void OpenGLShader::SetFloat(const char* name, const float value) const
	{
		glUniform1f(glGetUniformLocation(m_RendererID_, name), value);
	}

	void OpenGLShader::SetVec2(const char* name, const glm::vec2& vec2) const 
	{
		glUniform2f(glGetUniformLocation(m_RendererID_, name), vec2.x, vec2.y);
	}

	void OpenGLShader::SetVec3(const char* name, const glm::vec3& vec3) const
	{
		//glUniform3f(glGetUniformLocation(m_RendererID_, name), vec3.x, vec3.y, vec3.z);
		glUniform3fv(glGetUniformLocation(m_RendererID_, name), 1, glm::value_ptr(vec3));
	}

	void OpenGLShader::SetVec4(const char* name, const glm::vec4& vec4) const 
	{
		//glUniform4f(glGetUniformLocation(m_RendererID_, name), vec4.x, vec4.y, vec4.z, vec4.w);
		glUniform4fv(glGetUniformLocation(m_RendererID_, name), 1, glm::value_ptr(vec4));
	}

	void OpenGLShader::SetMat4(const char* name, const glm::mat4& mat4) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID_, name), 1, GL_FALSE, glm::value_ptr(mat4));
	}
}