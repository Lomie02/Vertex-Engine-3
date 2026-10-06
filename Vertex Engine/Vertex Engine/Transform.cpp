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

void VertexEngine::Transform::SetParent(std::weak_ptr<VertexEngine::Transform> _parent)
{

	// Store old position
	glm::mat4 oldWorldMatrix = m_WorldMatrix;
	auto newParent = _parent.lock();

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
		SetFromMatrix(oldWorldMatrix);
		return;
	}

	// set up child relations
	if (auto parent = _parent.lock()) {

		if (parent.get() == this)
			return;

		glm::mat4 newLocalMatrix = glm::inverse(parent->GetWorldMatrix()) * oldWorldMatrix;

		if (!SetFromMatrix(newLocalMatrix)) return;

		m_Parent = _parent;

		parent->m_Children.push_back(shared_from_this());
	}
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
}

glm::mat4 VertexEngine::Transform::GetWorldMatrix() const
{
	return m_WorldMatrix;
}
