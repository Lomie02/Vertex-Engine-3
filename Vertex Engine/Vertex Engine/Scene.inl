#pragma once
#include "GameObject.h"

// Returns the first loaded gameobject of type.
template<typename T>
T* VertexEngine::Scene::FindComponetOfType(bool IncludeInActive) {

	static_assert(std::is_base_of<VertexEngine::Component, T>::value, "VERTEX WARNING: Illegal Type in GetComponent");

	for (auto& ent : m_GameObjects) {

		if (!ent->IsActive() && !IncludeInActive) continue;

		for (auto& comp : ent->GetComponents()) {

			if (auto cast = dynamic_cast<T*>(comp.get())) {
				return cast;
			}
		}
	}

	return nullptr;
}