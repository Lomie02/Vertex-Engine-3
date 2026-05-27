#include "pch.h"
#include "WindowAPI.h"

VertexEngine::WindowAPI::WindowAPI(VertexEngine::Window* _window)
{
	m_EngineWindow = _window;
	std::string e = "Vertex Error : Window API failed! Window was bad.";

	if (!m_EngineWindow)
		throw std::runtime_error(e.c_str());
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
