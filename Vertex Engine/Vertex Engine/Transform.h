#pragma once
#include "Component.h"
#include <glm.hpp>
#include <vector>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <memory>
#include <gtx/quaternion.hpp>
namespace VertexEngine {

	class Scene;

	class Transform : public VertexEngine::Component, public std::enable_shared_from_this<Transform>
	{
		friend class Scene;
	public:

		void SetPosition(const glm::vec3& _position); // Set transform position
		void SetRotation(const glm::quat& _rotation); // Set transform rotation in quats
		void SetAngleAxis(float _angle, const glm::vec3& _axis); // Set rotation using angle axis
		void SetEulerRotation(const glm::vec3& _angle); // Set rotation in Euler Angles
		void SetScale(const glm::vec3& _scale); // Set transform scale

		glm::vec3 GetPosition() const { return m_Position; } // Get transform position
		glm::quat GetRotation() const { return m_Rotation; } // Get transform rotation in quat
		glm::vec3 GetEulerRotation() const; // Get transform rotatiin in Euler Angles
		glm::vec3 GetScale() const { return m_Scale; } // Get transform scale

		void SetParent(VertexEngine::Transform* _parent); // Set transform parent
		std::weak_ptr<VertexEngine::Transform> GetParent() const; // Get the tranforms parent
		const std::vector<std::weak_ptr<VertexEngine::Transform>>& GetChildren() const; // get all the children of transform

		bool HasParent() const; // Does transform have a parent
		glm::mat4 GetWorldMatrix() const; // Get the world matrix

		glm::vec3 GetForward() { return m_Rotation * glm::vec3(0, 0, -1); } // Get forward vector
		glm::vec3 GetUp() { return m_Rotation * glm::vec3(0, 1, 0 ); } // Get Up vector
		glm::vec3 GetDown() { return m_Rotation * glm::vec3(0, -1, 0); } // Get down vector
		glm::vec3 GetRight() { return m_Rotation * glm::vec3(1, 0, 0); } // Get Right vector
		glm::vec3 GetLeft() { return m_Rotation * glm::vec3(-1, 0, 0); } // Get Left vector

	private:
		glm::vec3 m_Position{0.0f};
		glm::quat m_Rotation{1.0f,0.0f,0.0f,0.0f};
		glm::vec3 m_Scale{ 1.0f};

		bool SetFromMatrix(const glm::mat4& _matrix); // Decompose the matrix back to vector space
		glm::mat4 GetLocalMatrix() const; // Locak space matrix
		void SetWorldMatrix(const glm::mat4& _matrix); // Sets the world matrix

		std::weak_ptr<Transform> m_Parent; // Parented Transform
		std::vector<std::weak_ptr<Transform>> m_Children; // Tranforms children

		glm::mat4 m_WorldMatrix{1.0f}; // World Matrix
		bool m_IsWorldMatrixValid = false;
	};
}

