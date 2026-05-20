#pragma once
#include "Component.h"
namespace VertexEngine {

	class VertexBehaviour : public VertexEngine::Component {
	public:
		// Vertex Behaviour is the base behavour script that all custom player scripts must inherit from.
		// This is done to allow Vertex Engine to determine what scripts are player created & what are factory.
		virtual ~VertexBehaviour() = default;
	};
}