#pragma once
#include "EngineContext.h"
#include "Scene.h"
#include <functional>

namespace VertexEngine {
	class SceneManager
	{
	public:
		SceneManager(VertexEngine::EngineContext* _ctx);

		std::shared_ptr<Scene> CreateScene(std::string _name,bool _load = false);
		void LoadScene(std::shared_ptr<Scene> _scene); // Load in a scene & take ownership
		void LoadScene(const std::string& _name);

		std::shared_ptr<Scene> GetScene(const std::string& _name) const;
		bool HasScene(const std::string& _name) const;

		void OnUpdate(); // Updates scene core
		void OnFixedUpdate(); // Updates scenes fixed update
		void OnUpdateTransforms(); // Updates all scene object transforms

		void ProcessCleanUp(); // called after all update processing & deletes any pending objects set for deletion.
		bool MoveSceneOrder(std::string _name, size_t _index); // Change a scenes order.

		std::function<void(Scene*)> OnSceneChanged; // Called when scene changes
		const std::vector<std::string>& GetSceneOrder() const { return m_SceneOrder; } // Return the scene order

	private:
		VertexEngine::EngineContext* m_Context; // Engine Context for scenes

		std::shared_ptr<Scene> m_ActiveScene; // The Active Scene
		std::shared_ptr<Scene> m_QueuedScene; // Scenes in the queue

		std::unordered_map<std::string, std::shared_ptr<Scene>> m_SceneRegister; // Registery of all scenes
		std::vector<std::string> m_SceneOrder; // The order of scene
		bool m_FirstSceneLoaded = false; // Flag to determine if the very first scene has been loaded.
	};
}

