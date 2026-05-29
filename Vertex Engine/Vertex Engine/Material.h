#pragma once
#include <string>
#include <memory>
#include "Texture.h"
namespace VertexEngine {

	struct Material {

		std::string m_Name;
		std::shared_ptr<VertexEngine::Texture> m_AlbedoMap;
		uint32_t id;
	};

}