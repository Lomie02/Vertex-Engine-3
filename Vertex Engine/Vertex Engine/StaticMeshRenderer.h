#pragma once
#include "Component.h"

namespace VertexEngine {


	class StaticMeshRenderer : public VertexEngine::Component
	{
	public:

		// Declare this component as a renderable
		virtual VertexEngine::ComponentFlags GetFlags() const override {
			return VertexEngine::ComponentFlags::Renderable;
		}

	};
}

