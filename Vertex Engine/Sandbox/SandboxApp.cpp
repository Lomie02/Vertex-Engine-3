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
		dum->GetComponenet<VertexEngine::Transform>()->SetPosition(glm::vec3(5.0f, 5.0f, 0.0f));
	}

	if (auto ent = m_MyObject.lock()) {
		ent->GetComponenet<VertexEngine::Transform>()->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

		/*if (auto dum = m_DummyObject.lock()) {
			dum->GetComponenet<VertexEngine::Transform>()->SetParent(ent->GetComponenet<VertexEngine::Transform>());
		}*/
	}

	if (auto cam = m_Camera.lock()) {
		cam->AddComponenet<VertexEngine::Camera>();
		cam->GetComponenet<VertexEngine::Transform>()->SetPosition(glm::vec3(0.0f, 0.7f, 10.0f));


	}

	m_Sphere = m_EngineAssetManager->Get<VertexEngine::Model>("Sphere");
	m_Cube = m_EngineAssetManager->Get<VertexEngine::Model>("Cube");

	if (auto dum = m_DummyObject.lock()) {

		if (auto obj = m_MyObject.lock()) {
			dum->GetComponenet<VertexEngine::Transform>()->SetParent(obj->GetComponenet<VertexEngine::Transform>());
		}
	}
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
