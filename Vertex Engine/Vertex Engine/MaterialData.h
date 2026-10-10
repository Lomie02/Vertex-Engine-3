#pragma once
#include <string>
#include <memory>
#include "Texture.h"
#include <glm.hpp>

namespace VertexEngine {

	struct MaterialData {

		std::string m_Name;

		// Base properties values
		glm::vec4 m_AlbedoColour{ 1.0f };
		float m_Metallic = 0.0f;
		float m_Roughness = 1.0f;

		// PRB texture properties
		std::string m_AlbedoMap;
		std::string m_AlbedoOverride;
		std::string m_MetallicMap;
		std::string m_RoughnessMap;
		std::string m_NormalMap;
	};
}