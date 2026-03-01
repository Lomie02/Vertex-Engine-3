#pragma once
#include "Shader.h"
#include "RenderableType.h"
#include <string>
namespace VertexEngine {
	struct Renderable {
		//TODO: Add all render stuff
		std::string Name;
		std::shared_ptr<Shader> m_ShaderOveride;
		RenderableType m_Type;
	};
}