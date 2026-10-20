#include "pch.h"
#include "SceneManager.h"
#include <algorithm>
VertexEngine::SceneManager::SceneManager(VertexEngine::EngineContext* _ctx)
{
	if (!_ctx)
		throw std::runtime_error("VERTEX ERROR: Scene Manager context is bad.");

	m_Context = _ctx;
}

std::shared_ptr<VertexEngine::Scene> VertexEngine::SceneManager::CreateScene(std::string _name, bool _load)
{

	// Check that scene name is not empty
	if (_name.empty()) throw std::runtime_error("VERTEX ERROR: Scene name cannot be empty");

	auto potentialScene = m_SceneRegister.find(_name);

	// Check if a scene already shares the same name
	if (potentialScene != m_SceneRegister.end()) throw std::runtime_error("VERTEX ERROR: A scene with this name is already registered");

	std::shared_ptr<Scene> scene = std::make_shared<Scene>(_name);

	m_SceneRegister.emplace(_name, scene); // Register scene
	m_SceneOrder.push_back(_name); // Push the scene into order

	if (_load)
		LoadScene(scene);

	return scene;
}

void VertexEngine::SceneManager::LoadScene(std::shared_ptr<Scene> _scene)
{
	if (!_scene)
		throw std::runtime_error("VERTEX ERROR: Scene cannot be null.");


	bool registered = false;

	for (const auto& [name, scene] : m_SceneRegister) {

		if (scene == _scene) {

			registered = true;
			break;
		}
	}

	if (!registered)
		throw std::runtime_error("VERTEX ERROR: Scene needs to be registered before being loaded.");

	m_FirstSceneLoaded = true;
	m_QueuedScene = _scene;
}

void VertexEngine::SceneManager::LoadScene(const std::string& _name)
{
	auto scene = GetScene(_name);

	if (!scene) {
		throw std::runtime_error("VERTEX ERROR: Scene is not registered: " + _name);
	}

	LoadScene(scene);
}

std::shared_ptr<VertexEngine::Scene> VertexEngine::SceneManager::GetScene(const std::string& _name) const
{

	auto it = m_SceneRegister.find(_name);

	if (it == m_SceneRegister.end()) return nullptr;

	return it->second;
}

bool VertexEngine::SceneManager::HasScene(const std::string& _name) const
{
	return m_SceneRegister.find(_name) != m_SceneRegister.end();
}

void VertexEngine::SceneManager::OnUpdate()
{
	// If not scene was loaded before the update loop, load the first scene by default.
	if (!m_FirstSceneLoaded && !m_ActiveScene && !m_QueuedScene && !m_SceneRegister.empty()) {
		LoadScene(m_SceneOrder.front());
	}

	// If scene has changed set the scene.
	if (m_QueuedScene && m_QueuedScene != m_ActiveScene) {

		m_ActiveScene = m_QueuedScene;
		m_QueuedScene.reset();

		if (m_ActiveScene) {
			m_ActiveScene->Init(m_Context);
			m_ActiveScene->UpdateTransforms();
		}

		if (OnSceneChanged)
			OnSceneChanged(m_ActiveScene.get());
	}
	else {
		m_QueuedScene.reset();
	}


	if (m_ActiveScene)
		m_ActiveScene->OnUpdate();
}

void VertexEngine::SceneManager::OnFixedUpdate()
{
	if (m_ActiveScene)
		m_ActiveScene->OnFixedUpdate();
}

void VertexEngine::SceneManager::OnUpdateTransforms()
{
	if (m_ActiveScene)
		m_ActiveScene->UpdateTransforms();
}

void VertexEngine::SceneManager::ProcessCleanUp()
{
	if (m_ActiveScene)
		m_ActiveScene->DeletePendingObjects();
}

bool VertexEngine::SceneManager::MoveSceneOrder(std::string _name, size_t _index)
{

	auto it = std::find(m_SceneOrder.begin(), m_SceneOrder.end(), _name);

	if (it == m_SceneOrder.end()) return false;

	if (_index >= m_SceneOrder.size()) return false;

	std::string sceneName = *it;

	m_SceneOrder.erase(it);

	m_SceneOrder.insert(m_SceneOrder.begin() + _index, sceneName);

	return true;
}
