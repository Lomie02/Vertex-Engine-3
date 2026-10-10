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
	m_Scene2 = m_EngineSceneManager->CreateScene("Scene2");

	m_MyObject = m_Scene->CreateGameObject("My Object");
	m_Camera = m_Scene->CreateGameObject("Camera");

	m_DummyObject = m_Scene2->CreateGameObject("Dummy");
	m_Scene2Camera = m_Scene2->CreateGameObject("Camera");

	if (auto dum = m_DummyObject.lock()) {
		dum->GetTransform()->SetPosition(glm::vec3(5.0f, 5.0f, 0.0f));
	}

	if (auto ent = m_MyObject.lock()) {
		ent->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

		/*if (auto dum = m_DummyObject.lock()) {
			dum->GetComponenet<VertexEngine::Transform>()->SetParent(ent->GetComponenet<VertexEngine::Transform>());
		}*/
	}

	if (auto cam = m_Camera.lock()) {
		cam->AddComponenet<VertexEngine::Camera>();
		cam->GetTransform()->SetPosition(glm::vec3(0.0f, 0.7f, 10.0f));
	}

	if (auto cam = m_Scene2Camera.lock()) {
		cam->AddComponenet<VertexEngine::Camera>();
		cam->GetTransform()->SetPosition(glm::vec3(0.0f, 0.7f, 10.0f));
	}

	m_Sphere = m_EngineAssetManager->Get<VertexEngine::Model>("Sphere");
	m_Cube = m_EngineAssetManager->Get<VertexEngine::Model>("Cube");

}

void SandboxApp::OnStart()
{
	RenameApplication("My Game");

	if (auto ent = m_MyObject.lock())
		ent->AddComponenet<VertexEngine::TestComp>();

	if (auto ent = m_DummyObject.lock()) {
		ent->AddComponenet<VertexEngine::StaticMeshRenderer>();
		ent->GetComponenet<VertexEngine::StaticMeshRenderer>()->SetMesh(m_Sphere);
	}

	if (auto ent = m_MyObject.lock()) {
		ent->AddComponenet<VertexEngine::StaticMeshRenderer>();
		ent->GetComponenet<VertexEngine::StaticMeshRenderer>()->SetMesh(m_Cube);
	}
}

void SandboxApp::OnUpdate()
{

}
