#pragma once
#include "Component.h"

namespace VertexEngine {

	class StaticMeshRenderer : public Component
	{
	public:

		virtual ComponentFlags GetFlags() const override {
			return ComponentFlags::Renderable;
		}

	};
}

