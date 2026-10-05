#pragma once
#include "Shader.h"
#include "RenderableType.h"
#include <string>
#include "Model.h"
#include "Transform.h"
namespace VertexEngine {
	struct Renderable {
		std::string Name;
		VertexEngine::Transform* m_Transform;
		std::shared_ptr<VertexEngine::Model> m_Models;
		std::shared_ptr<Shader> m_ShaderOverride;
		RenderableType m_Type;
	};
}