#pragma once
#include "Asset.h"
#include <glm.hpp>
#include <matrix.hpp>
namespace VertexEngine {
	class Shader : public VertexEngine::Asset {
	public:
		Shader() = default;

		void Load(const std::string& _path) override; // load the shader
		const std::string& GetShaderSource() const { return  m_ShaderSource; } // Get the shader itself.
		void    SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);
	private:
		std::string m_ShaderSource; // shader cpu data
	};
}