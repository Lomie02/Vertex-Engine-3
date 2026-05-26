#pragma once
#include "Scene.h"
#include <algorithm>
#include "Component.h"

// Add a component to the gameobject while also returning a ptr.
template<typename T, typename... Args>
T* VertexEngine::GameObject::AddComponenet(Args&&... args) {

	static_assert(std::is_base_of<VertexEngine::Component, T>::value, "VERTEX WARNING: Illegal Type in AddComponenet");

	auto comp = std::make_shared<T>(std::forward<Args>(args)...);
	comp->gameObject = this;

	T* raw = comp.get();
	m_Components.push_back(std::move(comp));
	raw->OnAttach(this);

	m_Scene->OnComponentAdded(raw);

	//TODO: Make scene register components.

	return raw;
}

// Get the component of type given.
template<typename T>
T* VertexEngine::GameObject::GetComponenet() {

	static_assert(std::is_base_of<VertexEngine::Component, T>::value, "VERTEX WARNING: Illegal Type in GetComponenet");

	for (auto& comp : m_Components) {
		if (auto cast = dynamic_cast<T*>(comp.get())) {
			return cast;
		}
	}

	return nullptr;
}


// Returns a list of all gameobjects of type.
template<typename T>
std::vector<T*> VertexEngine::GameObject::GetComponenetsOfType() {

	static_assert(std::is_base_of<VertexEngine::Component, T>::value, "VERTEX WARNING: Illegal Type in GetComponenetsOfType");

	std::vector<T*> list;

	for (auto& comp : m_Components) {
		if (auto cast = dynamic_cast<T*>(comp.get())) {
			list.push_back(cast);
		}
	}

	return list;
}