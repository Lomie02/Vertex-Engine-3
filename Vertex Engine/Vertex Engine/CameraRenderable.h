#pragma once
#include <glm.hpp>
namespace VertexEngine {

	struct CameraRenderable
	{
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		bool m_IsValid = false;
	};

}
