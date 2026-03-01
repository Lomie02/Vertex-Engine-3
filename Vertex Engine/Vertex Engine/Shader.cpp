#include "pch.h"
#include "Shader.h"
#include <fstream>
#include <iostream>

void VertexEngine::Shader::Load(const std::string& _path)
{
	std::ifstream file(_path);

	if (!file.is_open()) {
		m_AssetLoaded = false;
		return;
	}

	// Clear old shader data
	m_ShaderSource.clear();

	std::string line;

	// Upload all the shader code into the asset.
	while (std::getline(file, line)) {
		m_ShaderSource += line + "\n";
		std::cout << line << std::endl;
	}

	// Shader loaded
	m_AssetLoaded = true;

}
