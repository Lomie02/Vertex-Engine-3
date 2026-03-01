#include "SandboxApp.h"
#include "Texture.h"

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

	if (m_EngineAssetManager->Get<VertexEngine::Shader>("gl_default_shader_vertex")) {
		std::cout << "Loaded Shader!" << std::endl;
	}
	else {
		std::cout << "No Shader!" << std::endl;
	}

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
