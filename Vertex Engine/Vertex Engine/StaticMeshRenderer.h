#pragma once
#include "Component.h"
#include "Model.h"
namespace VertexEngine {

	class StaticMeshRenderer : public Component
	{
	public:

		virtual ComponentFlags GetFlags() const override {
			return ComponentFlags::Renderable;
		}

		// Set the static renderers mesh.
		void SetMesh(std::shared_ptr<VertexEngine::Model> _mesh) { m_Mesh = _mesh; }

		// Return the mesh the component has.
		std::shared_ptr<VertexEngine::Model> GetModel() { return m_Mesh; }

	private:
		std::shared_ptr<VertexEngine::Model> m_Mesh; // Mesh assigned to the static renderer
	};
}

