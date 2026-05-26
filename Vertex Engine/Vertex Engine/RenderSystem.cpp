#include "pch.h"
#include "RenderSystem.h"

//TODO: Replace all these place holder funcs with correct ones. These are for testing purposes.

VertexEngine::RenderSystem::RenderSystem(VertexEngine::Renderer* _renderAPI)
{
	m_MainlineRenderer = _renderAPI;
}

void VertexEngine::RenderSystem::OnSceneChanged(Scene* _scene)
{
	m_ActiveScene = _scene;

	std::cout << "Scene Changed" << std::endl;
}

void VertexEngine::RenderSystem::OnUpdate()
{
	if (!m_ActiveScene || !m_MainlineRenderer) return;

	m_MainlineRenderer->ClearFrame();
	// Starting a new frame
	m_MainlineRenderer->BeginFrame();


	// Get ready for render submissions

	for (auto* var : m_ActiveScene->GetRenderables())
	{
		Renderable mesh;
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
