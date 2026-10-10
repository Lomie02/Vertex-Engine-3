#include "TestComp.h"
void VertexEngine::TestComp::OnAwake(VertexEngine::EngineContext& _engine)
{

}
void VertexEngine::TestComp::OnStart(VertexEngine::EngineContext& _engine)
{
}

void VertexEngine::TestComp::OnUpdate(VertexEngine::EngineContext& _engine)
{

	// Rotation
	if (_engine.Input->GetKey(KeyCode::ArrowRight)) {

		m_Spin += 50 * _engine.Time->GetDeltaTime();
		gameObject->GetTransform()->SetAngleAxis(m_Spin, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if (_engine.Input->GetKey(KeyCode::ArrowLeft)) {

		m_Spin -= 50 * _engine.Time->GetDeltaTime();
		gameObject->GetTransform()->SetAngleAxis(m_Spin, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	// Vert

	if (_engine.Input->GetKey(KeyCode::ArrowDown)) {

		m_Movement -= 2 * _engine.Time->GetDeltaTime();

		glm::vec3 newPos = gameObject->GetTransform()->GetPosition();

		newPos.y = m_Movement;

		gameObject->GetTransform()->SetPosition(newPos);
	}

	if (_engine.Input->GetKey(KeyCode::ArrowUp)) {

		m_Movement += 2 * _engine.Time->GetDeltaTime();

		glm::vec3 newPos = gameObject->GetTransform()->GetPosition();

		newPos.y = m_Movement;

		gameObject->GetTransform()->SetPosition(newPos);
	}


	// Forward

	if (_engine.Input->GetKey(KeyCode::W)) {

		m_Forward -= 2 * _engine.Time->GetDeltaTime();

		glm::vec3 newPos = gameObject->GetTransform()->GetPosition();

		newPos.z = m_Forward;

		gameObject->GetTransform()->SetPosition(newPos);
	}

	if (_engine.Input->GetKey(KeyCode::S)) {

		m_Forward += 2 * _engine.Time->GetDeltaTime();

		glm::vec3 newPos = gameObject->GetTransform()->GetPosition();

		newPos.z = m_Forward;

		gameObject->GetTransform()->SetPosition(newPos);
	}


	// Quit Application
	if (_engine.Input->GetKey(KeyCode::Escape)) {

		_engine.Application->Quit();
	}

	if (_engine.Input->GetKey(KeyCode::Spacebar)) {
		_engine.SceneManager->LoadScene("Scene2");
	}
}
