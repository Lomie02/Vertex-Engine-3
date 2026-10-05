#include "TestComp.h"

void VertexEngine::TestComp::OnStart(VertexEngine::EngineContext& _engine)
{
}

void VertexEngine::TestComp::OnUpdate(VertexEngine::EngineContext& _engine)
{
	if (_engine.Input->GetKeyDown(KeyCode::Enter))
		std::cout << "Pressed!" << std::endl;

	if (_engine.Input->GetKeyDown(KeyCode::Tab)) {
		_engine.Window->SetWindowSize(500, 500);
		std::cout << "Window size changed!" << std::endl;
	}
}
