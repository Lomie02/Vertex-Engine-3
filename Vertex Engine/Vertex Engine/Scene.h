#pragma once
#include "EngineContext.h"
#include <vector>
#include "GameObject.h"
#include "StaticMeshRenderer.h"

namespace VertexEngine {
	class Scene
	{
		friend class SceneManager; // Make scene manager a friend so it can access the DeletePendingObjects() without giving sandbox access to it.

	public:
		Scene() {};
		Scene(std::string _name);
		void Init(VertexEngine::EngineContext* _ctx);
		// Set the scenes name
		void SetSceneName(std::string& _name) { m_Name = _name; }
		// Get the Scene name.
		std::string& GetSceneName() { return m_Name; }

		void OnUpdate(); // Runs all update loops on componets.
		void OnFixedUpdate(); // Runs the fixed update loop, this is done seperate from the main.

		std::weak_ptr<GameObject> CreateGameObject(std::string _name = "gameobject"); // Create a new gameobject

		void DestroyGameObject(std::weak_ptr<GameObject> _obj); // Destroy the given gameobject, return if successful.

		// Returns the first loaded gameobject of type.
		template<typename T>
		T* FindComponetOfType(bool IncludeInActive = false) {

			static_assert(std::is_base_of<VertexEngine::Component, T>::value, "VERTEX WARNING: Illegal Type in GetComponent");

			for (auto& ent : m_GameObjects) {

				if (!ent->IsActive() && !IncludeInActive) continue;

				for (auto& comp : ent->GetComponents()) {

					if (auto cast = dynamic_cast<T*>(comp.get())) {
						return cast;
					}
				}
			}

			return nullptr;
		}

		void RegisterStaticMesh(VertexEngine::StaticMeshRenderer* _mesh);

		std::weak_ptr<GameObject> FindGameObjectWithTag(std::string _tag); // Returns the first gameobject loaded with matching tag.

	private:
		void DeletePendingObjects(); // Deletes all objects after the update loop has been processed.

		std::string m_Name = "untitled"; // Scenes name

		bool m_HasSceneStarted = false; // Is scene hasnt started call start();
		std::vector<std::shared_ptr<GameObject>> m_GameObjects; // Gameobjects owned bt the scene
		std::vector<std::shared_ptr<GameObject>> m_PendingDeletion; // Gameobject waiting to be deleted.
		VertexEngine::EngineContext* m_Context; // Engine context.
		std::vector<StaticMeshRenderer*> m_RegisterdStaticMeshes;
	};
}

