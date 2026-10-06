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
		glm::vec3 m_Position{0.0f};
		glm::quat m_Rotation{1.0f,0.0f,0.0f,0.0f};
		glm::vec3 m_Scale{ 1.0f};


		void SetParent(std::weak_ptr<VertexEngine::Transform> _parent); // Set transform parent
		std::weak_ptr<VertexEngine::Transform> GetParent() const; // Get the tranforms parent
		const std::vector<std::weak_ptr<VertexEngine::Transform>>& GetChildren() const; // get all the children of transform

		bool HasParent() const; // Does transform have a parent
		glm::mat4 GetWorldMatrix() const; // Get the world matrix

	private:
		bool SetFromMatrix(const glm::mat4& _matrix); // Decompose the matrix back to vector space
		glm::mat4 GetLocalMatrix() const; // Locak space matrix
		void SetWorldMatrix(const glm::mat4& _matrix); // Sets the world matrix

		std::weak_ptr<Transform> m_Parent; // Parented Transform
		std::vector<std::weak_ptr<Transform>> m_Children; // Tranforms children

		glm::mat4 m_WorldMatrix{1.0f}; // World Matrix
	};
}

