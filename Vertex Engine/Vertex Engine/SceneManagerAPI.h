#pragma once
#include <string>

namespace VertexEngine {

	class SceneManager;

	class SceneManagerAPI
	{

	public:

		explicit SceneManagerAPI() {};
		explicit SceneManagerAPI(SceneManager* _sceneManager);

		void LoadScene(std::string _name);

	private:

		SceneManager* m_EngineSceneManager = nullptr;

	};
}

