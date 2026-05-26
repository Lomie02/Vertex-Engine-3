#pragma once
#include "EngineContext.h"
#include <vector>
#include "StaticMeshRenderer.h"

namespace VertexEngine {
	class Scene
	{
		friend class SceneManager; // Make scene manager a friend so it can access the DeletePendingObjects() without giving sandbox access to it.
		class GameObject;

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

		std::weak_ptr<VertexEngine::GameObject> CreateGameObject(std::string _name = "gameobject"); // Create a new gameobject

		void DestroyGameObject(std::weak_ptr<VertexEngine::GameObject> _obj); // Destroy the given gameobject, return if successful.

		// Returns the first loaded gameobject of type.
		template<typename T>
		T* FindComponetOfType(bool IncludeInActive = false);

		//NOTE: In future replace this to use component type for a singlular list.
		void RegisterStaticMesh(VertexEngine::StaticMeshRenderer* _mesh); // Register static meshes
		const std::vector<StaticMeshRenderer*>& GetRenderables() const { return m_RegisterdStaticMeshes; }

		void OnComponentAdded(VertexEngine::Component* _component);

		std::weak_ptr<VertexEngine::GameObject> FindGameObjectWithTag(std::string _tag); // Returns the first gameobject loaded with matching tag.

	private:
		void DeletePendingObjects(); // Deletes all objects after the update loop has been processed.

		std::string m_Name = "untitled"; // Scenes name

		bool m_HasSceneStarted = false; // Is scene hasnt started call start();
		std::vector<std::shared_ptr<VertexEngine::GameObject>> m_GameObjects; // Gameobjects owned bt the scene
		std::vector<std::shared_ptr<VertexEngine::GameObject>> m_PendingDeletion; // Gameobject waiting to be deleted.
		VertexEngine::EngineContext* m_Context; // Engine context.
		std::vector<StaticMeshRenderer*> m_RegisterdStaticMeshes; // Static Meshes.
	};
}

#include "Scene.inl"

