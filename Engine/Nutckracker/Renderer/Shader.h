#pragma once

#include <string>
#include <glm/gtc/type_ptr.hpp>


namespace NK {

	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		Shader(Shader&&);
		Shader& operator=(Shader&&);	

		Shader() = delete;
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		void Bind() const;
		void Unbind() const;
		bool IsCompiled() const { return m_IsCompiled_; }

		void SetMat4(const char* name, const glm::mat4& mat4) const;
		void SetInt(const char* name, const int value) const;
	private:
		uint32_t m_RendererID_;
		bool m_IsCompiled_ = false;
	};

}