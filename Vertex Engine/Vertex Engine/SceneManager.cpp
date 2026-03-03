#include "pch.h"
#include "SceneManager.h"

VertexEngine::SceneManager::SceneManager(VertexEngine::EngineContext* _ctx)
{
	if (!_ctx)
		throw std::runtime_error("VERTEX ERROR: Scene Manager context is bad.");

	m_Context = _ctx;
}

std::shared_ptr<VertexEngine::Scene> VertexEngine::SceneManager::CreateScene(std::string _name)
{
	std::shared_ptr<Scene> scene = std::make_shared<Scene>(_name);
	LoadScene(scene);
	return scene;
}

void VertexEngine::SceneManager::LoadScene(std::shared_ptr<Scene> _scene)
{
	m_QueuedScene = _scene;
}

void VertexEngine::SceneManager::OnUpdate()
{
	if (m_QueuedScene && m_QueuedScene != m_ActiveScene) {

		m_ActiveScene = m_QueuedScene;
		m_QueuedScene.reset();

		if (m_ActiveScene)
			m_ActiveScene->Init(m_Context);
	}

	if (m_ActiveScene)
		m_ActiveScene->OnUpdate();
}

void VertexEngine::SceneManager::OnFixedUpdate()
{
	if (m_ActiveScene)
		m_ActiveScene->OnFixedUpdate();
}

void VertexEngine::SceneManager::ProcessCleanUp()
{
	if (m_ActiveScene)
		m_ActiveScene->DeletePendingObjects();
}
