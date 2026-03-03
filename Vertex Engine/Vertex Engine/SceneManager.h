#pragma once
#include "EngineContex.h"
#include "Scene.h"

namespace VertexEngine {
	class SceneManager
	{
	public:
		SceneManager(VertexEngine::EngineContext* _ctx);

		std::shared_ptr<Scene> CreateScene(std::string _name);
		void LoadScene(std::shared_ptr<Scene> _scene); // Load in a scene & take ownership

		void OnUpdate(); // Updates scene core
		void OnFixedUpdate(); // Updates scenes fixed update

		void ProcessCleanUp(); // called after all update processing & deletes any pending objects set for deletion.

	private:
		VertexEngine::EngineContext* m_Context;

		std::shared_ptr<Scene> m_ActiveScene;
		std::shared_ptr<Scene> m_QueuedScene;
	};
}

