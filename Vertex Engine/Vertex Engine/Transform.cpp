#include "pch.h"
#include "Transform.h"
#include <gtx/matrix_decompose.hpp>


bool VertexEngine::Transform::SetFromMatrix(const glm::mat4& _matrix)
{
	glm::vec3 scale;
	glm::vec3 position;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::quat rotation;

	if (!glm::decompose(_matrix, scale, rotation, position, skew, perspective)) {
		return false;
	}

	m_Position = position;
	m_Rotation = glm::normalize(rotation);
	m_Scale = scale;

	return true;
}

glm::mat4 VertexEngine::Transform::GetLocalMatrix() const
{
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, m_Position);
	trans *= glm::mat4_cast(m_Rotation);
	trans = glm::scale(trans, m_Scale);

	return trans;
}

void VertexEngine::Transform::SetPosition(const glm::vec3& _position)
{
	m_Position = _position;
	m_IsWorldMatrixValid = false;
}

void VertexEngine::Transform::SetRotation(const glm::quat& _rotation)
{
	m_Rotation = _rotation;
	m_IsWorldMatrixValid = false;
}

void VertexEngine::Transform::SetAngleAxis(float _angle, const glm::vec3& _axis)
{
	float axisLength = glm::length(_axis);

	if (axisLength <= 0.0001f) return;
	m_Rotation = glm::angleAxis(glm::radians(_angle), _axis / axisLength);

	m_IsWorldMatrixValid = false;
}

void VertexEngine::Transform::SetEulerRotation(const glm::vec3& _angle)
{
	m_Rotation = glm::quat(glm::radians(_angle));
	m_IsWorldMatrixValid = false;
}

void VertexEngine::Transform::SetScale(const glm::vec3& _scale)
{
	m_Scale = _scale;
	m_IsWorldMatrixValid = false;
}

glm::vec3 VertexEngine::Transform::GetEulerRotation() const
{
	return glm::degrees(glm::eulerAngles(m_Rotation));
}

void VertexEngine::Transform::SetParent(VertexEngine::Transform* _parent)
{

	// Store old position
	glm::mat4 oldWorldMatrix = m_WorldMatrix;
	bool preserveWorld = m_IsWorldMatrixValid;

	std::shared_ptr<Transform> newParent;

	if (_parent) {
		newParent = _parent->shared_from_this();
	}

	// Check that old & new parent isnt itself
	if (newParent && newParent.get() == this) return;

	// Remove
	if (auto oldParent = m_Parent.lock()) {

		auto& children = oldParent->m_Children;

		children.erase(std::remove_if(children.begin(), children.end(), [this](const std::weak_ptr<Transform>& child) { auto lockedChild = child.lock(); return !lockedChild || lockedChild.get() == this; }), children.end());
	}

	m_Parent.reset();

	// check if parent is valid
	if (!newParent) {

		if (preserveWorld)
			SetFromMatrix(oldWorldMatrix);

		return;
	}

	if (preserveWorld) {

		glm::mat4 newLocalMatrix = glm::inverse(newParent->GetWorldMatrix()) * oldWorldMatrix;

		if (!SetFromMatrix(newLocalMatrix)) return;
	}

	m_Parent = newParent;

	newParent->m_Children.push_back(shared_from_this());
}

std::weak_ptr<VertexEngine::Transform> VertexEngine::Transform::GetParent() const
{
	return m_Parent;
}

const std::vector<std::weak_ptr<VertexEngine::Transform>>& VertexEngine::Transform::GetChildren() const
{
	return m_Children;
}

bool VertexEngine::Transform::HasParent() const
{
	return !m_Parent.expired();
}

void VertexEngine::Transform::SetWorldMatrix(const glm::mat4& _matrix)
{
	m_WorldMatrix = _matrix;
	m_IsWorldMatrixValid = true;
}

glm::mat4 VertexEngine::Transform::GetWorldMatrix() const
{
	return m_WorldMatrix;
}
