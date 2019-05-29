#pragma once

#include "RetroGF/Core.h"
#include "RGFpch.h"
#include "GLM/glm/glm.hpp"

// Shader interface.

// The 'Create' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.

namespace RGF {


	class RGF_API Shader {

		public :
			Shader(){}
			virtual ~Shader(){}


			virtual void Init() = 0;
			virtual void ShutDown() = 0;

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual void LoadFromSrc(const std::string& filepath) = 0;


			virtual void SetUniform4f(const std::string& uniformName, const glm::vec4& values) = 0;
			virtual void SetUniform1i(const std::string& uniformName, const int value) = 0;
			virtual void SetUniformMatrix(const std::string& uniformName, const glm::mat4& matrix) = 0;
		public :
			static Shader* Create();

	};

}