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
	m_DummyObject = m_Scene->CreateGameObject("Dummy");
	m_Camera = m_Scene->CreateGameObject("Camera");

	if (auto dum = m_DummyObject.lock()) {
		dum->GetComponenet<VertexEngine::Transform>()->m_Position = glm::vec3(5.0f, 10.0f, 0.0f);
	}

	if (auto ent = m_MyObject.lock()) {
		ent->GetComponenet<VertexEngine::Transform>()->m_Position = glm::vec3(1.0f, 1.0f, 1.0f);

		if (auto dum = m_DummyObject.lock()) {
			dum->GetComponenet<VertexEngine::Transform>()->SetParent(ent->GetComponenet<VertexEngine::Transform>());
		}
	}

	if (auto cam = m_Camera.lock()) {
		cam->AddComponenet<VertexEngine::Camera>();
		cam->GetComponenet<VertexEngine::Transform>()->m_Position = glm::vec3(0.0f, 0.0f, 20.0f);
		cam->GetComponenet<VertexEngine::Camera>()->m_FieldOfView = 100.0f;

		
	}

	m_Model = m_EngineAssetManager->Get<VertexEngine::Model>("Cube");
}

void SandboxApp::OnStart()
{
	RenameApplication("My Game");



	if (auto ent = m_MyObject.lock())
		ent->AddComponenet<VertexEngine::TestComp>();

	if (auto ent = m_DummyObject.lock()) {
		ent->AddComponenet<VertexEngine::StaticMeshRenderer>();
		ent->GetComponenet<VertexEngine::StaticMeshRenderer>()->SetMesh(m_Model);
	}

	if (auto ent = m_MyObject.lock()) {
		ent->AddComponenet<VertexEngine::StaticMeshRenderer>();
		ent->GetComponenet<VertexEngine::StaticMeshRenderer>()->SetMesh(m_Model);
	}
}

void SandboxApp::OnUpdate()
{	
	if (auto cube = m_MyObject.lock()) {
		std::cout << "Cube Pos: " << cube->GetComponenet<VertexEngine::Transform>()->m_Position.x << " | " << cube->GetComponenet<VertexEngine::Transform>()->m_Position.y << " | " << cube->GetComponenet<VertexEngine::Transform>()->m_Position.z << std::endl;
	}

	if (auto cube = m_Camera.lock()) {
		std::cout << "Camera Pos: " << cube->GetComponenet<VertexEngine::Transform>()->m_Position.x << " | " << cube->GetComponenet<VertexEngine::Transform>()->m_Position.y << " | " << cube->GetComponenet<VertexEngine::Transform>()->m_Position.z << std::endl;
	}

}
