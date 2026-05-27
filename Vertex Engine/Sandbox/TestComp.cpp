#include "TestComp.h"

void VertexEngine::TestComp::OnStart(VertexEngine::EngineContext& _engine)
{
}

void VertexEngine::TestComp::OnUpdate(VertexEngine::EngineContext& _engine)
{
	if (_engine.Input->GetKeyDown(KeyCode::Enter))
		std::cout << "Pressed!" << std::endl;

	if (_engine.Input->GetKeyDown(KeyCode::Spacebar))
		_engine.Window->SetFullscreen(!_engine.Window->IsFullscreen());

}
