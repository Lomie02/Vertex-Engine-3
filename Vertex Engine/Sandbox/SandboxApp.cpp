#include "SandboxApp.h"

// Required for the engine to link to the sandbox.
VertexEngine::Application* CreateApp() {

	return new SandboxApp();
}

// Sandbox Code 

void SandboxApp::OnAwake()
{
	SetApplicationFullscreenMode(false);
}

void SandboxApp::OnStart()
{
	RenameApplication("My Game");

}

void SandboxApp::OnUpdate()
{
	if (m_EngineInputSystem->GetKeyDown(KeyCode::A)) {
		std::cout << "Pressed A!" << std::endl;
	}

	if (m_EngineInputSystem->GetKeyDown(KeyCode::W)) {
		std::cout << "Pressed W!" << std::endl;
	}

	if (m_EngineInputSystem->GetKeyDown(KeyCode::D)) {

		SetApplicationFullscreenMode(!IsWindowFullscreen());
	}

	if (m_EngineInputSystem->GetKeyDown(KeyCode::Escape)) {
		Quit();
	}
}
