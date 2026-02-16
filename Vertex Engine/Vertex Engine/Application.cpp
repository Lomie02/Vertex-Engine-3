#include "pch.h"
#include "Application.h"

VertexEngine::Application::Application()
{
}

VertexEngine::Application::~Application()
{
}

void VertexEngine::Application::Execute()
{
	m_IsEngineRunning = true;

	while (m_IsEngineRunning) {
		OnUpdate();
	}
}
