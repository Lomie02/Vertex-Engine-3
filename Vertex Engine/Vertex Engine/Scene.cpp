#include "pch.h"
#include "Scene.h"
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"
#include <glm.hpp>
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
	// This is separate from OnUpdate because fixed update is called differently.

	for (auto& ent : m_GameObjects) {

		if (!ent->IsActive()) continue; // Only update components if gameobject is active

		for (auto& comp : ent->GetComponents()) // Update all context into components
		{
			if (!comp->IsEnabled()) continue;
			comp->OnFixedUpdate(*m_Context);
		}
	}

}

void VertexEngine::Scene::UpdateTransforms()
{
	for (auto& object : m_GameObjects) {
		Transform* transform = object->GetComponenet<Transform>();

		if (!transform)
			continue;

		if (!transform->HasParent()) {
			UpdateTransformHierarchy(transform, glm::mat4(1.0f));
		}
	}
}

void VertexEngine::Scene::UpdateTransformHierarchy(Transform* _transform, const glm::mat4& _parentWorld)
{

	glm::mat4 local = _transform->GetLocalMatrix();

	_transform->SetWorldMatrix(_parentWorld * local);

	for (const auto& Wchild : _transform->GetChildren()) {

		if (auto child = Wchild.lock()) {
			UpdateTransformHierarchy(child.get(), _transform->GetWorldMatrix());
		}
	}
}



std::weak_ptr<VertexEngine::GameObject> VertexEngine::Scene::CreateGameObject(std::string _name)
{
	auto ent = std::make_shared<VertexEngine::GameObject>(this);

	ent->SetName(_name);
	m_GameObjects.push_back(ent);
	ent->AddComponenet<Transform>();

	return ent;
}

void VertexEngine::Scene::DestroyGameObject(std::weak_ptr<VertexEngine::GameObject> _obj)
{
	// Push the pending object into a queue to be deleted at the end of the update cycle.
	if (auto ptr = _obj.lock())
		m_PendingDeletion.push_back(ptr);
}

void VertexEngine::Scene::RegisterStaticMesh(VertexEngine::StaticMeshRenderer* _mesh)
{
	if (!_mesh) return;

	m_RegisterdStaticMeshes.push_back(_mesh);
}

void VertexEngine::Scene::RegisterCamera(VertexEngine::Camera* _camera)
{
	if (!_camera) return;
	m_RegisteredCameras.push_back(_camera);
}

void VertexEngine::Scene::OnComponentAdded(VertexEngine::Component* _component)
{
	if (!_component) return;

	if (_component->HasFlags(ComponentFlags::Renderable)) {
		m_RegisterdStaticMeshes.push_back(static_cast<StaticMeshRenderer*>(_component));
	}
	else if (_component->HasFlags(ComponentFlags::Camera)) {
		m_RegisteredCameras.push_back(static_cast<Camera*>(_component));
	}
}

void VertexEngine::Scene::OnComponentRemoved(VertexEngine::Component* _component)
{

	if (_component->HasFlags(ComponentFlags::Renderable)) {
		std::erase(m_RegisterdStaticMeshes, _component);
	}
	else if (_component->HasFlags(ComponentFlags::Camera)) {
		std::erase(m_RegisteredCameras, _component);
	}
}

std::weak_ptr<VertexEngine::GameObject> VertexEngine::Scene::FindGameObjectWithTag(std::string _tag)
{
	// Go through all gameobjects & find the one with matching tag.
	for (auto& ent : m_GameObjects) {
		if (ent->GetTag() == _tag)
			return ent;
	}

	return std::weak_ptr<VertexEngine::GameObject>(); // if no gameobject was found return an empty weak ptr
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
