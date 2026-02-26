#include "pch.h"
#include "Application.h"
#include "GlWindow.h"
#include "GlfwInputSystem.h"
#include <memory>
#include <iostream>
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

			// Update delta time 
			if (m_EngineClock)
				m_EngineClock->ConfigureDeltaTime();

			if (m_EngineInputSystem)
				m_EngineInputSystem->UpdateInputLogs();

			OnUpdate();
			OnLateUpdate();

			// calculate fixed delta & call fixed update loop
			if (m_EngineClock) {
				while (m_EngineClock->GetFixedDeltaTime() >= m_EngineClock->GetTimeStep()) {
					OnFixedUpdate();
					m_EngineClock->ResetFixedDelta();
				}
			}

			// Render system begins the render process.
			if (m_EngineRenderSystem)
				m_EngineRenderSystem->OnUpdate();

			// Update the window polling. This always should happen last.
			if (m_GameWindow != nullptr)
				m_GameWindow->OnUpdate();
		}
	else
		std::cout << "Window & Input Failed!: Graphics API Does not exist. Falling Back to safe mode." << std::endl;
}

void VertexEngine::Application::Quit()
{
	m_IsEngineRunning = false;
	if (m_GameWindow)
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

bool VertexEngine::Application::IsWindowFullscreen()
{
	if (m_GameWindow)
		return m_GameWindow->IsWindowFullscreen();
	return false;
}

float VertexEngine::Application::GetDelta() const
{
	if (m_EngineClock)
		return m_EngineClock->GetDeltaTime();

	return 0.0f;
}

float VertexEngine::Application::GetFixedDelta() const
{
	if (m_EngineClock)
		return m_EngineClock->GetFixedDeltaTime();

	return 0.0f;
}

float VertexEngine::Application::GetFramesPerSecond() const
{
	if (m_EngineClock)
		return m_EngineClock->GetAppFramesPerSecond();

	return 0.0f;
}

void VertexEngine::Application::InitProps()
{
	// Create the window for application.
	m_EngineGraphics = VertexEngine::GraphicsAPI::OpenGL;

	m_GameWindow = m_EngineBackend.CreateWindow(m_EngineGraphics, 500, 500);

	if (m_GameWindow)
		m_EngineInputSystem = m_EngineBackend.CreateInput(m_EngineGraphics, m_GameWindow.get());
	else
		std::cout << "Window & Input Failed!: Graphics API Does not exist." << std::endl;


	// engine clock 
	m_EngineClock = std::make_unique<EngineTime>();
	m_EngineRenderSystem = std::make_unique<RenderSystem>();

	OnAwake();
}
