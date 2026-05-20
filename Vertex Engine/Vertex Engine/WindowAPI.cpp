#include "pch.h"
#include "WindowAPI.h"

VertexEngine::WindowAPI::WindowAPI(VertexEngine::Window* _window)
{
	m_EngineWindow = _window;
	if (!m_EngineWindow)
		std::runtime_error("Vertex Error: Window API failed! Window was bad.");
}

void VertexEngine::WindowAPI::SetFullscreen(bool _state)
{
	if (m_EngineWindow)
		m_EngineWindow->SetFullscreen(_state);
}

bool VertexEngine::WindowAPI::IsFullscreen()
{
	if (m_EngineWindow)
		return m_EngineWindow->IsWindowFullscreen();

	return false;
}

void VertexEngine::WindowAPI::SetVsync(bool _state)
{
	if (m_EngineWindow)
		m_EngineWindow->SetVerticalSync(_state);
}
