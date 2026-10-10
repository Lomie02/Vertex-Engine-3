#include "pch.h"
#include "SceneManagerAPI.h"
#include "SceneManager.h"

VertexEngine::SceneManagerAPI::SceneManagerAPI(SceneManager* _sceneManager)
{
	m_EngineSceneManager = _sceneManager;

}

void VertexEngine::SceneManagerAPI::LoadScene(std::string _name)
{
	if (m_EngineSceneManager)
		m_EngineSceneManager->LoadScene(_name);
}
