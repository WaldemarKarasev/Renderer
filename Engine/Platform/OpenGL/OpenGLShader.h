#pragma once

#include "Nutckracker/Renderer/Shader.h"

#include <glad/glad.h>
#include <unordered_map>
#include <vector>

namespace NK {
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader();
        
        #if 0
        OpenGLShader(OpenGLShader&& shader) noexcept;
        OpenGLShader& operator=(OpenGLShader&& shader) noexcept;
        #endif

        virtual const std::string& GetName() const override { return m_Name_;}
        virtual void Bind() const override;
        virtual void Unbind() const override;

		virtual void SetInt(const char* name, const int value) const override;
		virtual void SetFloat(const char* name, const float value) const override;
        virtual void SetVec2(const char* name, const glm::vec2& vec2) const override;
		virtual void SetVec3(const char* name, const glm::vec3& vec3) const override;
        virtual void SetVec4(const char* name, const glm::vec4& vec4) const override;
        virtual void SetMat4(const char* name, const glm::mat4& mat4) const override;
    
    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        
        //void Compile(const std::unordered_map<GLenum, std::string> shaderSources);
        void CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources);
        void CompileOrGetOpenGLBinaries();
        void CreateProgram();
        void Reflect(GLenum stage, const std::vector<uint32_t>& shaderData);


	private:
		uint32_t m_RendererID_;   
        std::string m_FilePath_ = "/home/pingvinus/prog/game_eng/Renderer";
        std::string m_Name_;

        std::unordered_map<GLenum, std::vector<uint32_t>> m_VulkanSPIRV_;
        std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV_;

        std::unordered_map<GLenum, std::string> m_OpenGLSourceCode_;

    };
}