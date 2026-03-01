#include "pch.h"
#include "RenderSystem.h"

//TODO: Replace all these place holder funcs with correct ones. These are for testing purposes.

VertexEngine::RenderSystem::RenderSystem(VertexEngine::Renderer* _renderAPI)
{
	m_MainlineRenderer = _renderAPI;
}

void VertexEngine::RenderSystem::OnUpdate()
{
	// Starting a new frame
	if (m_MainlineRenderer)
		m_MainlineRenderer->BeginFrame();

	if (m_MainlineRenderer)
		m_MainlineRenderer->ClearFrame();

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
