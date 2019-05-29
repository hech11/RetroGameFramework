#pragma once

#include "RetroGF/Rendering/API/Shader.h"

// OpenGL's implementation of 'Shader.h'

namespace RGF {

	class RGF_API GLShader : public Shader {

		public:
			struct ShaderSource {
				std::string VertexShaderStr;
				std::string FragmentShaderStr;
			};

			GLShader();
			~GLShader();

			void Init() override;
			void ShutDown() override;



			void Bind() const override;
			void Unbind() const override;

			void LoadFromSrc(const std::string& filepath) override;


			void SetUniform4f(const std::string& uniformName, const glm::vec4& values) override;
			void SetUniform1i(const std::string& uniformName, const int value) override;
			void SetUniformMatrix(const std::string& uniformName, const glm::mat4& matrix) override;


		private :
			unsigned int m_RendererID;

			std::string m_Filepath;
		private :
			int m_GetUniformLocation(const std::string& name);
			ShaderSource PraseShader(const std::string shaderFile);
			unsigned int CreateShader(unsigned int type, const std::string& shaderSource);
	};

}