#pragma once
#include <vector>
#include <type_traits>
#include <string>

namespace VertexEngine {

	class Componenet;
	class Scene;

	class GameObject {
	public:

		GameObject() {};
		GameObject(VertexEngine::Scene* _scene) { m_Scene = _scene; };

		// Set the name of the gameobject
		void SetName(std::string& _name) { m_Name = _name; }

		// Get the name of the gameobject.
		std::string& GetName() { return m_Name; }

		// Get the gameobjects current id.
		unsigned int GetID() { return m_Id; }

		// Set the gameobjects active state
		void SetActive(bool _state) { m_IsActive = _state; }

		// Is the gameobject active.
		bool IsActive() { return m_IsActive; }

		// Add a componenet to the gameobject while also returning a ptr.
		template<typename T, typename... Args>
		T* AddComponenet(Args&&... args) {

			static_assert(std::is_base_of<VertexEngine::Componenet, T>::value, "VERTEX WARNING: Illegal Type in AddComponenet");

			auto comp = std::make_shared<T>(std::forward<Args>(args)...);
			comp->gameObject = this;

			T* raw = comp.get();
			m_Componenets.push_back(std::move(comp));

			std::cout << "Component has been added" << std::endl;
			return raw;
		}

		// Get the componenet of type given.
		template<typename T>
		T* GetComponenet() {

			static_assert(std::is_base_of<VertexEngine::Componenet, T>::value, "VERTEX WARNING: Illegal Type in GetComponenet");

			for (auto& comp : m_Componenets) {
				if (auto cast = dynamic_cast<T*>(comp.get())) {
					return cast;
				}
			}

			return nullptr;
		}

		// Returns a list of all the componenets on the gameobject.
		const std::vector<std::shared_ptr<VertexEngine::Componenet>>& GetComponenets() const {
			return m_Componenets;
		}

		// Returns a list of all gameobjects of type.
		template<typename T>
		std::vector<T*> GetComponenetsOfType() {

			static_assert(std::is_base_of<VertexEngine::Componenet, T>::value, "VERTEX WARNING: Illegal Type in GetComponenetsOfType");

			std::vector<T*> list;

			for (auto& comp : m_Componenets) {
				if (auto cast = dynamic_cast<T*>(comp.get())) {
					list.push_back(cast);
				}
			}

			return list;
		}

		VertexEngine::Scene* GetScene() { return m_Scene; }

	private:
		std::string m_Name = "Gameobject"; // Gameobjects name
		unsigned int m_Id = 0; // Gameobjects Id;
		bool m_IsActive = true; // Active State of gameobject.
		VertexEngine::Scene* m_Scene = nullptr; // The scene the gameobject is apart of.

		std::vector<std::shared_ptr<VertexEngine::Componenet>> m_Componenets; // Componenets attached to gameobject.
	};
}