#include "pch.h"
#include "Transform.h"

glm::mat4 VertexEngine::Transform::GetMatrix()
{
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, m_Position);
	trans *= glm::mat4_cast(m_Rotation);
	trans = glm::scale(trans, m_Scale);

	return trans;
}
