#include "VulkanShader.hpp"
#include "nkpch.h"

#include <fstream>
//#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>


#include "VulkanRenderBackend.hpp"
#include "VKContext.hpp"

namespace NK {

	namespace detail {
		static uint32_t ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
			{
				return VK_SHADER_STAGE_VERTEX_BIT;
			}
			if (type == "fragment" || type == "pixel")
			{
				return VK_SHADER_STAGE_FRAGMENT_BIT;
			}

			NK_CORE_ASSERT(false, "Unknown shader type");
			return 0;
		}

		static shaderc_shader_kind GLShaderStageToShaderC(uint32_t stage)
		{
			switch (stage)
			{
			case VK_SHADER_STAGE_VERTEX_BIT: return shaderc_glsl_vertex_shader;
			case VK_SHADER_STAGE_FRAGMENT_BIT: return shaderc_glsl_fragment_shader;
			}
			NK_CORE_ASSERT(false, "Unknown shaderc_shader_kind type!");
			return static_cast<shaderc_shader_kind>(0);
		}
		
		static const char* GLShaderStageToString(uint32_t stage)
		{
			switch (stage)
			{
			case VK_SHADER_STAGE_VERTEX_BIT: return "VK_VERTEX_SHADER";
			case VK_SHADER_STAGE_FRAGMENT_BIT: return "VK_FRAGMENT_SHADER";
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

		static const char* GLShaderStageCacheVulkanFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case VK_SHADER_STAGE_VERTEX_BIT: return ".cached_Vulkan.vert";
			case VK_SHADER_STAGE_FRAGMENT_BIT: return ".cached_Vulkan.frag";
			}
			NK_CORE_ASSERT(false, "Unsupported stage for Cached Vulkan File Extension!");
			return "";
		}

		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case VK_SHADER_STAGE_VERTEX_BIT: return ".cached_vulkan.vert";
			case VK_SHADER_STAGE_FRAGMENT_BIT: return ".cached_vulkan.frag";
			}
			NK_CORE_ASSERT(false, "Unsupported stage for Cached Vulkan File Extension!");
			return "";		
		}
	}

	VulkanShader::VulkanShader(const std::string& filepath)
	: m_FilePath_(filepath)
	{
		detail::CreateCacheSirectoryIfNeeded();
		
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		
		{
			//Timer timer; //TODO: timer implementation
			CompileVulkanBinaries(shaderSources);
			CreateShaderModule();
			//NK_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
		}

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name_ = filepath.substr(lastSlash, count);
	}

    VulkanShader::VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	: m_Name_(name)
	{
		NK_TRACE("shader name: {0}", name);
		//NK_TRACE("shader vertexSrc: {0}", vertexSrc);
		//NK_TRACE("shader fragmentSrc: {0}", fragmentSrc);
		
		std::unordered_map<uint32_t, std::string> sources;
		sources[VK_SHADER_STAGE_VERTEX_BIT] = vertexSrc;
		sources[VK_SHADER_STAGE_FRAGMENT_BIT] = fragmentSrc;
		CompileVulkanBinaries(sources);
		CreateShaderModule();
	}

	VulkanShader::~VulkanShader()
	{
	}
	
	#if 0
	VulkanShader::VulkanShader(VulkanShader&& shader) noexcept
	{
		m_RendererID_ = shader.m_RendererID_;
		shader.m_RendererID_ = 0;
	}

	VulkanShader& VulkanShader::operator=(VulkanShader&& shader) noexcept
	{
		m_RendererID_ = shader.m_RendererID_;
		shader.m_RendererID_ = 0;

		return *this;

	}
	#endif

	std::string VulkanShader::ReadFile(const std::string& filepath)
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

	std::unordered_map<uint32_t, std::string> VulkanShader::PreProcess(const std::string& source)
	{
		std::unordered_map<uint32_t, std::string> shaderSources;

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

	void VulkanShader::CompileVulkanBinaries(const std::unordered_map<uint32_t, std::string>& shaderSources)
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		const bool optimize = false;
		//const bool optimize = true;

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

			//NK_CORE_TRACE("shaderFilePath - {0}", shaderFilePath.string());
			//NK_CORE_TRACE("cachedPath - {0}", cachedPath.string());

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
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, detail::GLShaderStageToShaderC(stage), m_FilePath_.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					NK_CORE_ERROR(module.GetErrorMessage());
					NK_CORE_ASSERT(false, "CompileVulkanBinaries: Module compilation error!");
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

	
	void VulkanShader::CreateShaderModule()
	{

		NK_CORE_TRACE("{0}", m_VulkanSPIRV_[VK_SHADER_STAGE_VERTEX_BIT].size());
		NK_CORE_TRACE("{0}", m_VulkanSPIRV_[VK_SHADER_STAGE_FRAGMENT_BIT].size());
		// Creating m_VertexShaderModule_	
		VkShaderModuleCreateInfo vertexCreateInfo{};
        vertexCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        vertexCreateInfo.codeSize = m_VulkanSPIRV_[VK_SHADER_STAGE_VERTEX_BIT].size()*4;
        vertexCreateInfo.pCode = reinterpret_cast<const uint32_t*>(m_VulkanSPIRV_[VK_SHADER_STAGE_VERTEX_BIT].data());

        if(vkCreateShaderModule(vkContext.device, &vertexCreateInfo, nullptr, &m_VertexShaderModule_) != VK_SUCCESS)
        { 
			NK_CORE_ERROR("Vertex ShaderModule creation failure!");
			NK_CORE_ASSERT(false, "Failed to create vertex shader module");
        }

		// Creating m_FragmentShaderModule_
		VkShaderModuleCreateInfo fragmentCreateInfo{};
        fragmentCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        fragmentCreateInfo.codeSize = m_VulkanSPIRV_[VK_SHADER_STAGE_FRAGMENT_BIT].size()*4;
        fragmentCreateInfo.pCode = reinterpret_cast<const uint32_t*>(m_VulkanSPIRV_[VK_SHADER_STAGE_FRAGMENT_BIT].data());

        if(vkCreateShaderModule(vkContext.device, &fragmentCreateInfo, nullptr, &m_FragmentShaderModule_) != VK_SUCCESS)
        { 
            throw std::runtime_error("failed to create shader module");
        }
	}

	void VulkanShader::Reflect(uint32_t stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		NK_CORE_TRACE("VulkanShader::Reflect - {0} {1}", detail::GLShaderStageToString(stage), m_FilePath_);
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



}