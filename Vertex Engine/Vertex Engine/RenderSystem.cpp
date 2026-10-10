#include "pch.h"
#include "RenderSystem.h"
#include "Transform.h"
//TODO: Replace all these place holder funcs with correct ones. These are for testing purposes.

VertexEngine::RenderSystem::RenderSystem(VertexEngine::Renderer* _renderAPI)
{
	m_MainlineRenderer = _renderAPI;
}

void VertexEngine::RenderSystem::OnSceneChanged(Scene* _scene)
{
	m_ActiveScene = _scene;
}

void VertexEngine::RenderSystem::OnUpdate()
{
	if (!m_ActiveScene || !m_MainlineRenderer) return;

	// Starting a new frame

	m_MainlineRenderer->BeginFrame();

	m_MainlineRenderer->ClearFrame();
	// Submit the camera
	float aspect =  static_cast<float>(1920.0f) / static_cast<float>(1080.0f);

	for (auto* var : m_ActiveScene->GetCameras())
	{
		CameraRenderable camera;

		camera.m_ViewMatrix = glm::inverse(var->gameObject->GetComponenet<Transform>()->GetWorldMatrix());
		camera.m_ProjectionMatrix = glm::perspective(glm::radians(var->m_FieldOfView), aspect, var->m_NearClip, var->m_FarClip);

		m_MainlineRenderer->SubmitCamera(camera);
		break;
	}

	// Get ready for render submissions
	for (auto* var : m_ActiveScene->GetRenderables())
	{
		if (!var->GetModel()) continue;

		Renderable mesh;

		mesh.m_Models = var->GetModel();
		mesh.ModelMatrix = var->gameObject->GetComponenet<Transform>()->GetWorldMatrix();

		mesh.Name = var->gameObject->GetName();
		mesh.m_Type = RenderableType::Mesh_3D;

		m_MainlineRenderer->Submit(mesh);
	}

	// Render the frame
	m_MainlineRenderer->Render();

	// End of the current frame
	m_MainlineRenderer->EndFrame();
}

void VertexEngine::RenderSystem::InitProps()
{
}
