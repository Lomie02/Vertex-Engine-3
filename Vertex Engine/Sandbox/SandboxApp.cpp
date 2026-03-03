#include "SandboxApp.h"
#include "Texture.h"
#include <memory>
#include "TestComp.h"

// Required for the engine to link to the sandbox.
VertexEngine::Application* CreateApp() {

	return new SandboxApp();
}

// Sandbox Code 

void SandboxApp::OnAwake()
{
	SetApplicationFullscreenMode(false);

	m_Scene = m_EngineSceneManager->CreateScene("My Scene");

	m_MyObject = m_Scene->CreateGameObject("My Object");


}

void SandboxApp::OnStart()
{
	RenameApplication("My Game");

	if (auto ent = m_MyObject.lock())
		std::cout << ent->GetName() << std::endl;

	if (auto ent = m_MyObject.lock())
		ent->AddComponenet<VertexEngine::TestComp>();


}

void SandboxApp::OnUpdate()
{
}
