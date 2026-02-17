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
	m_Timer += GetDeltaTime();
	std::cout << (int)m_Timer << std::endl;
}
