#pragma once

#include "Nutckracker/Renderer/Shader.h"

namespace NK {
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader();
        
        OpenGLShader(OpenGLShader&& shader) noexcept;
        OpenGLShader& operator=(OpenGLShader&& shader) noexcept;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        void SetMat4(const char* name, const glm::mat4& mat4) const;
		void SetInt(const char* name, const int value) const;
		void SetFloat(const char* name, const float value) const;
		void SetVec3(const char* name, const glm::vec3 vec3) const;
        

	private:
		uint32_t m_RendererID_;   
    };
}