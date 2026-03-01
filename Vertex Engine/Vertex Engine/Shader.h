#pragma once
#include "Asset.h"
namespace VertexEngine {
	class Shader : public VertexEngine::Asset {
	public:
		Shader() = default;

		void Load(const std::string& _path) override; // load the shader
		const std::string& GetShaderSource() const { return  m_ShaderSource; } // Get the shader itself.
	private:
		std::string m_ShaderSource; // shader cpu data
	};
}