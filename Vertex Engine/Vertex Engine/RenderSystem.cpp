#include "pch.h"
#include "RenderSystem.h"

//TODO: Replace all these place holder funcs with correct ones. These are for testing purposes.

VertexEngine::RenderSystem::RenderSystem()
{
	InitProps();
}

void VertexEngine::RenderSystem::OnUpdate()
{
	if (m_MainlineRenderer)
		m_MainlineRenderer->RenderFrame();
}

void VertexEngine::RenderSystem::InitProps()
{
	m_MainlineRenderer = std::make_unique<Renderer>();
}
