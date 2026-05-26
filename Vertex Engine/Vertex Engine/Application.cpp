#include "pch.h"
#include "Application.h"
#include "GlWindow.h"
#include "GlfwInputSystem.h"
#include <memory>
#include <iostream>
#include "Shader.h"
VertexEngine::Application::Application()
{
	InitProps();// Initalize all systems & check that they actually are created.
}

VertexEngine::Application::~Application()
{
}

void VertexEngine::Application::Execute()
{
	OnAwake();
	OnStart();

	m_IsEngineRunning = true;

	// Change the window name to indicate safe mode is enabled.
	if (m_EngineHealth == VertexEngine::EngineMode::SafeMode) {
		std::string title = GetApplicationName();
		RenameApplication(title + " (Safe-Mode)");
	}

	// Engine life
	if (m_EngineHealth == VertexEngine::EngineMode::NormalMode)
		while (!m_EngineWindow->IsWindowClosed()) {

			// Update delta time 
			if (m_EngineClock)
				m_EngineClock->ConfigureDeltaTime();

			if (m_EngineInputSystem)
				m_EngineInputSystem->UpdateInputLogs();

			if (m_EngineSceneManager)
				m_EngineSceneManager->OnUpdate();

			// Sandbox update functions.
			OnUpdate();
			OnLateUpdate();

			// calculate fixed delta & call fixed update loop
			if (m_EngineClock) {
				while (m_EngineClock->GetFixedDeltaTime() >= m_EngineClock->GetTimeStep()) {
					OnFixedUpdate();
					m_EngineClock->ResetFixedDelta();

					// Update scene manager
					if (m_EngineSceneManager)
						m_EngineSceneManager->OnFixedUpdate();
				}
			}

			// Tell the scene manager to make scenes delete any gameobjects waiting to be deleted by user.
			if (m_EngineSceneManager)
				m_EngineSceneManager->ProcessCleanUp();

			// Render system begins the render process.
			if (m_EngineRenderSystem)
				m_EngineRenderSystem->OnUpdate();

			// Update the window polling. This always should happen last.
			if (m_EngineWindow != nullptr)
				m_EngineWindow->OnUpdate();

			// Render system begins the render process.
			if (m_EngineRenderSystem)
				m_EngineRenderSystem->OnUpdate();
		}
	else
		while (m_IsEngineRunning) {
			// Engine is still runing here but during safemode the engine will not update core systems. Only scenes.

			if (m_EngineSceneManager)
				m_EngineSceneManager->OnUpdate();

			// Sandbox update functions.
			OnUpdate();
			OnLateUpdate();

			// Tell the scene manager to make scenes delete any gameobjects waiting to be deleted by user.
			if (m_EngineSceneManager)
				m_EngineSceneManager->ProcessCleanUp();

			// Render system begins the render process.
			if (m_EngineRenderSystem)
				m_EngineRenderSystem->OnUpdate();

			// Update the window polling. This always should happen last.
			if (m_EngineWindow != nullptr)
				m_EngineWindow->OnUpdate();

			// Render system begins the render process.
			if (m_EngineRenderSystem)
				m_EngineRenderSystem->OnUpdate();
		}
}

void VertexEngine::Application::Quit()
{
	m_IsEngineRunning = false;
	if (m_EngineWindow)
		m_EngineWindow->CloseWindow();
}

void VertexEngine::Application::RenameApplication(std::string _nameApp)
{
	if (m_EngineWindow)
		m_EngineWindow->SetWindowName(_nameApp);
}

std::string VertexEngine::Application::GetApplicationName()
{
	if (m_EngineWindow)
		return m_EngineWindow->GetWindowName();

	return "untitled";
}

void VertexEngine::Application::SetApplicationFullscreenMode(bool _fullscreenMode)
{
	if (m_EngineWindow)
		m_EngineWindow->SetFullscreen(_fullscreenMode);
}

void VertexEngine::Application::SetVSync(bool _vSync)
{
	if (m_EngineWindow)
		m_EngineWindow->SetVerticalSync(_vSync);
}

bool VertexEngine::Application::IsWindowFullscreen()
{
	if (m_EngineWindow)
		return m_EngineWindow->IsWindowFullscreen();
	return false;
}

void VertexEngine::Application::SetEngineAPI(VertexEngine::GraphicsAPI _api)
{
	//TODO: Allow for API to switch at runtime.
	m_EngineGraphics = _api;
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

void VertexEngine::Application::SetRootPath(std::string _filePath)
{
	// Only set the file path if the asset manager exists
	if (m_EngineAssetManager)
		m_EngineAssetManager.get()->SetRootPath(_filePath);
}

void VertexEngine::Application::InitProps()
{
	// Create the Window
	SetEngineAPI(VertexEngine::GraphicsAPI::OpenGL);

	// Create asset manager
	m_EngineAssetManager = std::make_unique<AssetManager>();

	// Set default filepath
	std::string name = "Assets";
	m_EngineAssetManager.get()->SetRootPath(name);

	if (m_EngineAssetManager)
		m_EngineAssetManager->AutoLoadAll(name);

	// Create the Context menu.
	m_EngineContext = std::make_unique<EngineContext>();

	try
	{
		// Create core systems based on selected API
		m_EngineWindow = m_EngineBackend.CreateWindow(m_EngineGraphics, 500, 500);
		m_EngineBackend.CreateInput(m_EngineGraphics, m_EngineWindow.get());
		m_EngineRenderer = m_EngineBackend.CreateRenderer(m_EngineGraphics, m_EngineWindow.get(), m_EngineAssetManager.get());
		m_EngineInputSystem = m_EngineBackend.CreateInput(m_EngineGraphics, m_EngineWindow.get());

		// Create the render system & assigned the created renderer.
		m_EngineSceneManager = std::make_unique<SceneManager>(m_EngineContext.get());
		m_EngineRenderSystem = std::make_unique<RenderSystem>(m_EngineRenderer.get());

		// engine clock 
		m_EngineClock = std::make_unique<EngineTime>();
		// Create & link Context APIS
		m_EngineContext->Input = std::make_unique<InputAPI>(m_EngineInputSystem.get()); // Assign the input to the context menu.
		m_EngineContext->Window = std::make_unique<WindowAPI>(m_EngineWindow.get()); // Assign the Window to the context menu.
		m_EngineContext->Time = std::make_unique<TimeAPI>(m_EngineClock.get()); // Assign the Time class to the context menu.

	}
	catch (const std::exception& e) // If core systems fail to be created, enter safe-mode to allow the engine to continue to run scenes. Core systems will not be updated.
	{
		m_EngineHealth = VertexEngine::EngineMode::SafeMode;
		std::cout << "VERTEX ERROR: Core Systems failed: " << e.what() << " Entering SafeMode." << std::endl;

		// Reset ptrs
		m_EngineWindow.reset(); // App Window
		m_EngineInputSystem.reset(); // Input System
		m_EngineRenderer.reset(); // Renderer 

		m_EngineRenderSystem.reset(); // Render System
		m_EngineSceneManager.reset(); // Scene Manager
		m_EngineClock.reset(); // Time Class
	}

	// If both the scene manager & render system have been created allow the render system to subscrive to the scene changes.
	auto renderSystem = m_EngineRenderSystem.get();

	if (m_EngineSceneManager && m_EngineRenderSystem) {
		m_EngineSceneManager.get()->OnSceneChanged = [renderSystem](Scene* s) {
			renderSystem->OnSceneChanged(s);
			};
	}

	// Check Engine Context APIs 

	if (!m_EngineContext->Input) {
		m_EngineContext->Input = std::make_unique<InputAPI>();
		m_EngineHealth = VertexEngine::EngineMode::SafeMode;
		std::cout << "VERTEX ERROR: Core Systems failed: (INPUT API) Entering SafeMode." << std::endl;

	}
	if (!m_EngineContext->Time) {

		m_EngineContext->Time = std::make_unique<TimeAPI>();
		m_EngineHealth = VertexEngine::EngineMode::SafeMode;
		std::cout << "VERTEX ERROR: Core Systems failed: (TIME API) Entering SafeMode." << std::endl;
	}
	if (!m_EngineContext->Window)
	{
		m_EngineContext->Window = std::make_unique<WindowAPI>();
		std::cout << "VERTEX ERROR: Core Systems failed: (WINDOW API) Entering SafeMode." << std::endl;
		m_EngineHealth = VertexEngine::EngineMode::SafeMode;
	}


}