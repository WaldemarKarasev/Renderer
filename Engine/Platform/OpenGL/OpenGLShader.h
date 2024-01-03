#pragma once

#include "Nutckracker/Renderer/Shader.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

// TODO: delete this typedef
using GLenum = unsigned int;// unsigned int = GLenum;

namespace NK {
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader();
        
        OpenGLShader(OpenGLShader&& shader) noexcept;
        OpenGLShader& operator=(OpenGLShader&& shader) noexcept;

        virtual const std::string& GetName() const override { return m_Name_;}
        virtual void Bind() const override;
        virtual void Unbind() const override;

        void SetMat4(const char* name, const glm::mat4& mat4) const;
		void SetInt(const char* name, const int value) const;
		void SetFloat(const char* name, const float value) const;
		void SetVec3(const char* name, const glm::vec3 vec3) const;
    
    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string> shaderSources);

	private:
		uint32_t m_RendererID_;   
        std::string m_Name_;
    };
}