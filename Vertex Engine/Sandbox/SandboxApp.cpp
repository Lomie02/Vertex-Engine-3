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
	SetApplicationFullscreenMode(true);

	m_Scene = m_EngineSceneManager->CreateScene("My Scene");

	m_MyObject = m_Scene->CreateGameObject("My Object");
	m_Model = m_EngineAssetManager->Get<VertexEngine::Model>("Cube");

}

void SandboxApp::OnStart()
{
	RenameApplication("My Game");

	if (m_Model)
		std::cout << m_Model->modelName << std::endl;

	if (auto ent = m_MyObject.lock())
		ent->GetComponenet<VertexEngine::Transform>()->m_Rotation =
		glm::angleAxis(
			glm::radians(45.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

	if (auto ent = m_MyObject.lock())
		ent->GetComponenet<VertexEngine::Transform>()->m_Scale = glm::vec3(0.5f, 0.5f, 0.5f);

	if (auto ent = m_MyObject.lock())
		std::cout << ent->GetName() << std::endl;

	if (auto ent = m_MyObject.lock())
		ent->AddComponenet<VertexEngine::TestComp>();

	if (auto ent = m_MyObject.lock()) {
		ent->AddComponenet<VertexEngine::StaticMeshRenderer>();
		ent->GetComponenet<VertexEngine::StaticMeshRenderer>()->SetMesh(m_Model);
	}
}

void SandboxApp::OnUpdate()
{
}
