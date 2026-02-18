#pragma once
#include "EngineContex.h"

namespace VertexEngine {

	class Componenet {
	public:
		virtual ~Componenet() = default;

		virtual void OnAwake(VertexEngine::EngineContext& _engine) = 0; // called on init of scene.
		virtual void OnStart(VertexEngine::EngineContext& _engine) = 0; // called at the start of frame
		virtual void OnUpdate(VertexEngine::EngineContext& _engine) = 0; // called every frame.

		virtual void OnLateUpdate(VertexEngine::EngineContext& _engine) = 0; // called after update.
		virtual void OnFixedUpdate(VertexEngine::EngineContext& _engine) = 0; // called at a fixed interval for physics steps.

	protected:
		unsigned int m_ComponentID;
	};
}