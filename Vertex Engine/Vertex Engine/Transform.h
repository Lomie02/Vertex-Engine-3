#pragma once
#include "Component.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
namespace VertexEngine {

	class Transform : public VertexEngine::Component
	{
	public:
		glm::vec3 m_Position;
		glm::quat m_Rotation;
		glm::vec3 m_Scale;

		glm::mat4 GetMatrix();

	private:
		glm::mat4 m_LocalPositon;
		glm::mat4 m_WorldPositon;
	};
}

