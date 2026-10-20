#pragma once
#include "Component.h"
#include "VertexBehaviour.h"
#include <VertexAPI.h>
namespace VertexEngine {
	class TestComp : public VertexEngine::VertexBehaviour {
	public:

		void OnAwake(VertexEngine::EngineContext& _engine) override;
		void OnStart(VertexEngine::EngineContext& _engine) override;
		void OnUpdate(VertexEngine::EngineContext& _engine) override;

		void skipScene();
	private:

		VertexEngine::VertexEvent<> m_Event;

		float m_Spin;
		float m_Movement;
		float m_Forward;
	};
}
