#include "pch.h"
#include "ApplicationAPI.h"
#include "Application.h"

VertexEngine::ApplicationAPI::ApplicationAPI(Application* _app)
{
	m_EngineApplication = _app;

	std::string e = "Vertex Error : Application API failed! Application was bad.";

	if (!m_EngineApplication)
		throw std::runtime_error(e.c_str());
}

void VertexEngine::ApplicationAPI::Quit()
{
	if (m_EngineApplication)
		m_EngineApplication->Quit();
}

bool VertexEngine::ApplicationAPI::OpenProgram(std::string _name)
{
	if (!m_EngineApplication) return false;

	return m_EngineApplication->OpenProgram(_name);
}
