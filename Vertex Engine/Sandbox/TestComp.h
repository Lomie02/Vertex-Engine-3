#pragma once
#include "Component.h"
#include "VertexBehaviour.h"

namespace VertexEngine {
	class TestComp : public VertexEngine::VertexBehaviour {
	public:

		void OnAwake(VertexEngine::EngineContext& _engine) override;
		void OnStart(VertexEngine::EngineContext& _engine) override;
		void OnUpdate(VertexEngine::EngineContext& _engine) override;


	private:

		float m_Spin;
		Transform* m_Trans;
		float m_Movement;
		float m_Forward;
	};
}
