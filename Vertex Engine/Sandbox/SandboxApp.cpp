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
	std::cout << (int)GetFramesPerSecond() << std::endl;
}
