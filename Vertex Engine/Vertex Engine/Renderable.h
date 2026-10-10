#pragma once
#include "Shader.h"
#include "RenderableType.h"
#include <string>
#include "Model.h"
namespace VertexEngine {
	struct Renderable {
		std::string Name;
		glm::mat4 ModelMatrix{ 1.0f };
		std::shared_ptr<VertexEngine::Model> m_Models;
		std::shared_ptr<Shader> m_ShaderOverride;
		RenderableType m_Type;
	};
}