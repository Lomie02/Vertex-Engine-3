#pragma once
#include "Componenet.h"
#include "VertexBehaviour.h"

namespace VertexEngine {
	class TestComp : public VertexEngine::VertexBehaviour {
	public:

		void OnStart(VertexEngine::EngineContext& _engine) override;
		void OnUpdate(VertexEngine::EngineContext& _engine) override;
	};
}
