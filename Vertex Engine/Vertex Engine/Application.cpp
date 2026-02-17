#include "pch.h"
#include "Application.h"
#include "GlWindow.h"

VertexEngine::Application::Application()
{
	InitProps();// Initalize all systems & check that they actually are created.
}

VertexEngine::Application::~Application()
{
}

void VertexEngine::Application::Execute()
{
	OnStart();

	m_IsEngineRunning = true;

	if (m_GameWindow)
		while (!m_GameWindow->IsWindowClosed()) {

			m_EngineClock->ConfigureDeltaTime();

			OnUpdate();
			OnLateUpdate();

			while (m_EngineClock->GetFixedDeltaTime() >= m_EngineClock->GetTimeStep()) {
				OnFixedUpdate();
				m_EngineClock->ResetFixedDelta();
			}


			// Update the window polling. This always should happen last.
			if (m_GameWindow != nullptr)
				m_GameWindow->OnUpdate();
		}
}

void VertexEngine::Application::Quit()
{
	m_IsEngineRunning = false;
	m_GameWindow->CloseWindow();
}

void VertexEngine::Application::RenameApplication(std::string _nameApp)
{
	if (m_GameWindow)
		m_GameWindow->SetWindowName(_nameApp);
}

void VertexEngine::Application::SetApplicationFullscreenMode(bool _fullscreenMode)
{
	if (m_GameWindow)
		m_GameWindow->SetFullscreen(_fullscreenMode);
}

void VertexEngine::Application::SetVSync(bool _vSync)
{
	if (m_GameWindow)
		m_GameWindow->SetVerticalSync(_vSync);
}

float VertexEngine::Application::GetDelta() const
{
	return m_EngineClock->GetDeltaTime();
}

float VertexEngine::Application::GetFixedDelta() const
{
	return m_EngineClock->GetFixedDeltaTime();
}

void VertexEngine::Application::InitProps()
{
	// Create the window for application.
	m_GameWindow = Window::Create();
	m_EngineClock = std::make_unique<EngineTime>();

	OnAwake();
}
