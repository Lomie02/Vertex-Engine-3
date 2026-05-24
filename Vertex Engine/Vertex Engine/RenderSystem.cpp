#include "pch.h"
#include "RenderSystem.h"

//TODO: Replace all these place holder funcs with correct ones. These are for testing purposes.

VertexEngine::RenderSystem::RenderSystem(VertexEngine::Renderer* _renderAPI)
{
	m_MainlineRenderer = _renderAPI;
}

void VertexEngine::RenderSystem::SetActiveScene(Scene* _activeScene)
{
	m_ActiveScene = _activeScene;
	//TODO: Add a check
}

void VertexEngine::RenderSystem::OnUpdate()
{
	if (m_MainlineRenderer)
		m_MainlineRenderer->ClearFrame();
	// Starting a new frame
	if (m_MainlineRenderer)
		m_MainlineRenderer->BeginFrame();


	// Get ready for render submissions




	Renderable renderSubmission;
	renderSubmission.Name = "Quit it";

	if (m_MainlineRenderer)
		m_MainlineRenderer->Submit(renderSubmission);

	// Render the frame
	if (m_MainlineRenderer)
		m_MainlineRenderer->Render();


	// End of the current frame
	if (m_MainlineRenderer)
		m_MainlineRenderer->EndFrame();
}

void VertexEngine::RenderSystem::InitProps()
{
}
