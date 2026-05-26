#pragma once
#include <vector>
#include <type_traits>
#include <string>

namespace VertexEngine {

	class Component;
	class Scene;

	class GameObject {
	public:

		GameObject() {};
		GameObject(VertexEngine::Scene* _scene) { m_Scene = _scene; };

		// Set the name of the gameobject
		void SetName(std::string& _name) { m_Name = _name; }

		// Get the name of the gameobject.
		std::string& GetName() { return m_Name; }

		// Set the gameobjects tag
		void SetTag(std::string _tag) { m_ObjectTag = _tag; }

		// Get the gameobjects tag
		std::string GetTag() { return m_ObjectTag; }

		// Get the gameobjects current id.
		unsigned int GetID() { return m_Id; }

		// Set the gameobjects active state
		void SetActive(bool _state) { m_IsActive = _state; }

		// Is the gameobject active.
		bool IsActive() { return m_IsActive; }

		// Add a componenet to the gameobject while also returning a ptr.
		template<typename T, typename... Args>
		T* AddComponenet(Args&&... args);

		// Get the component of type given.
		template<typename T>
		T* GetComponenet();

		// Returns a list of all the componenets on the gameobject.
		const std::vector<std::shared_ptr<VertexEngine::Component>>& GetComponents() const {
			return m_Components;
		}

		// Returns a list of all gameobjects of type.
		template<typename T>
		std::vector<T*> GetComponenetsOfType();

		VertexEngine::Scene* GetScene() { return m_Scene; }

	private:
		std::string m_Name = "Gameobject"; // Gameobjects name
		std::string m_ObjectTag = "untagged"; // Gameobjects tag

		unsigned int m_Id = 0; // Gameobjects Id;
		bool m_IsActive = true; // Active State of gameobject.
		Scene* m_Scene = nullptr; // The scene the gameobject is apart of.

		std::vector<std::shared_ptr<VertexEngine::Component>> m_Components; // Components attached to gameobject.
	};

}
#include "GameObject.inl";
