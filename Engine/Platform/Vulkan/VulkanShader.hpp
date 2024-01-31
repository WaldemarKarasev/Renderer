#pragma once

#include "Nutckracker/Renderer/Shader.hpp"

#include <vulkan/vulkan.h>
#include <unordered_map>
#include <vector>

namespace NK {
    class VulkanShader : public Shader
    {
    public:
        VulkanShader(const std::string& filepath);
        VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~VulkanShader();
        
        #if 1
        VulkanShader(VulkanShader&& shader) noexcept;
        VulkanShader& operator=(VulkanShader&& shader) noexcept;
        #endif

        virtual const std::string& GetName() const override { return m_Name_;}
        // Shaders will be bounded in pipeline object.
        //virtual void Bind() const override;
        //virtual void Unbind() const override;
    
    public:
    // Getters specific for VulkanShader
    VkShaderModule GetVertexShaderModule() const { return *m_VertexShaderModule_; }
    VkShaderModule GetFragmentShaderModule() const { return *m_FragmentShaderModule_; }

    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<uint32_t, std::string> PreProcess(const std::string& source);
        
        //void Compile(const std::unordered_map<GLenum, std::string> shaderSources);
        void CompileVulkanBinaries(const std::unordered_map<uint32_t, std::string>& shaderSources);
        void CreateShaderModule();
        void Reflect(uint32_t stage, const std::vector<uint32_t>& shaderData);


	private:  
        VkShaderModule* m_VertexShaderModule_ = nullptr;
        VkShaderModule* m_FragmentShaderModule_ = nullptr;

        std::string m_FilePath_ = "/home/pingvinus/prog/game_eng/Renderer";
        std::string m_Name_;

        std::unordered_map<uint32_t, std::vector<uint32_t>> m_VulkanSPIRV_;
        //std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV_;

        //std::unordered_map<GLenum, std::string> m_VulkanSourceCode_;

    };
}