#include "pch.h"
#include "Scene.h"
#include "Component.h"

VertexEngine::Scene::Scene(std::string _name)
{
	SetSceneName(_name);
}

void VertexEngine::Scene::Init(VertexEngine::EngineContext* _ctx)
{
	m_Context = _ctx;

	// On awake
	for (auto& ent : m_GameObjects) {

		if (!ent->IsActive()) continue; // Only update componets if gameobject is active

		for (auto& comp : ent->GetComponents()) // Update all context into components
			comp->OnAwake(*m_Context);
	}
}

void VertexEngine::Scene::OnUpdate()
{
	if (!m_HasSceneStarted) { // Start

		m_HasSceneStarted = true;

		for (auto& ent : m_GameObjects) {

			if (!ent->IsActive()) continue; // Only update componets if gameobject is active

			for (auto& comp : ent->GetComponents()) // Update all context into components
			{
				if (!comp->IsEnabled()) continue;
				comp->OnStart(*m_Context);
			}

		}
	}

	// Update
	for (auto& ent : m_GameObjects) {

		if (!ent->IsActive()) continue; // Only update componets if gameobject is active

		for (auto& comp : ent->GetComponents()) // Update all context into components
		{
			if (!comp->IsEnabled()) continue;
			comp->OnUpdate(*m_Context);
		}
	}

	// Late Update
	for (auto& ent : m_GameObjects) {

		if (!ent->IsActive()) continue; // Only update componets if gameobject is active

		for (auto& comp : ent->GetComponents()) // Update all context into components
		{
			if (!comp->IsEnabled()) continue;
			comp->OnFixedUpdate(*m_Context);
		}
	}
}

void VertexEngine::Scene::OnFixedUpdate()
{
	// This is seperate from OnUpdate because fixed update is called differently.

	for (auto& ent : m_GameObjects) {

		if (!ent->IsActive()) continue; // Only update componets if gameobject is active

		for (auto& comp : ent->GetComponents()) // Update all context into components
		{
			if (!comp->IsEnabled()) continue;
			comp->OnFixedUpdate(*m_Context);
		}
	}

}

std::weak_ptr<VertexEngine::GameObject> VertexEngine::Scene::CreateGameObject(std::string _name)
{
	auto ent = std::make_shared<GameObject>(this);

	ent->SetName(_name);
	m_GameObjects.push_back(ent);

	return ent;
}

void VertexEngine::Scene::DestroyGameObject(std::weak_ptr<GameObject> _obj)
{
	// Push the pending object into a queue to be deleted at the end of the update cycle.
	if (auto ptr = _obj.lock())
		m_PendingDeletion.push_back(ptr);
}

std::weak_ptr<VertexEngine::GameObject> VertexEngine::Scene::FindGameObjectWithTag(std::string _tag)
{
	// Go through all gameobjects & find the one with matching tag.
	for (auto& ent : m_GameObjects) {
		if (ent->GetTag() == _tag)
			return ent;
	}

	return std::weak_ptr<GameObject>(); // if no gameobject was found return an empty weak ptr
}

void VertexEngine::Scene::DeletePendingObjects()
{
	for (auto& ent : m_PendingDeletion) {

		for (auto& comp : ent->GetComponents()) // Call on destroy hook for all comps
			comp->OnDestroy();

		// Remove & delete the gameobject from the scene.
		auto it = std::remove(m_GameObjects.begin(), m_GameObjects.end(), ent);
		m_GameObjects.erase(it, m_GameObjects.end());
	}
	m_PendingDeletion.clear();
}
