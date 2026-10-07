#include "TestComp.h"
void VertexEngine::TestComp::OnStart(VertexEngine::EngineContext& _engine)
{
}

void VertexEngine::TestComp::OnUpdate(VertexEngine::EngineContext& _engine)
{
		m_Spin += 10 * _engine.Time->GetDeltaTime();

		gameObject->GetComponenet<Transform>()->m_Rotation =
			glm::angleAxis(
				glm::radians(m_Spin),
				glm::vec3(1.0f, 0.0f, 0.0f)
			);
}
