#pragma once

#include <string>
#include <unordered_map>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Nutckracker/Core.h"

namespace NK {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual const std::string& GetName() const = 0;

		static Shader* Create(const std::string& filepath);
		static Shader* Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		
		virtual void SetInt(const char* name, const int value) const = 0;
		virtual void SetFloat(const char* name, const float value) const = 0;
		virtual void SetVec2(const char* name, const glm::vec2& vec2) const = 0;
		virtual void SetVec3(const char* name, const glm::vec3& vec3) const = 0;
		virtual void SetVec4(const char* name, const glm::vec4& vec4) const = 0;
		virtual void SetMat4(const char* name, const glm::mat4& mat4) const = 0;
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, Shader* shader);
		void Add(Shader* shader);
		Shader* Load(const std::string& filepath);
		Shader* Load(const std::string name, const std::string& filepath);

		Shader* Get(const std::string& name);

		bool Exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, Shader*> m_Shaders_;
	};

}