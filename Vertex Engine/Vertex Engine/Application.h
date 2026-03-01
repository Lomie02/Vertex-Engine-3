#pragma once
/*
	Application for running
*/
#include "Window.h"
#include <iostream>
#include "EngineTime.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "EngineContex.h"

#include "BackendFactory.h"
#include "GraphicsAPI.h"
#include "EngineMode.h"
#include "AssetManager.h"

namespace VertexEngine {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Execute(); // Run the engine
		void Quit(); // Quit the application

	protected:
		// Engine main functions
		virtual void OnAwake() {} // Called on initalize
		virtual void OnStart() {} // Called before update begins
		virtual void OnUpdate() {} // Called Every frame
		virtual void OnLateUpdate() {} // Called later affter main update
		virtual void OnFixedUpdate() {} // Called Every frame at fixed intervals for physics.

		// Window Wrappers for Sandbox interactions.
		void RenameApplication(std::string _nameApp); // Rename the window of the application.
		void SetApplicationFullscreenMode(bool _fullscreenMode); // change the windows fullscreen
		void SetVSync(bool _vSync); // Set windows Vsync state
		bool IsWindowFullscreen(); // Is the screen fullscreen
		void SetEngineAPI(VertexEngine::GraphicsAPI _api); // Set the engines graphics API.

		// Engine Clock Wrappers
		float GetDelta() const; // Get the current delta time in seconds
		float GetFixedDelta() const; // Get the fixed delta
		float GetFramesPerSecond() const;

		// is only here for input testing until scene manager is added.
		std::unique_ptr<InputSystem> m_EngineInputSystem;
		std::unique_ptr<AssetManager> m_EngineAssetManager;

	private:
		
		//TODO: Implement engine health safemode fallback.
		EngineMode m_EngineHealth; // The current health state of the engine. If everything is init correctly it shall run as normal. If something fails it enters safemode for debugging. 
		GraphicsAPI m_EngineGraphics; // The graphics api the engine should be using for input/graphics
		BackendFactory m_EngineBackend; // Backend to create systems based off given api setting.

		std::unique_ptr<EngineTime> m_EngineClock; // Allows delta to be grabbed
		std::unique_ptr<Window> m_EngineWindow; // The window the engine uses.
		std::unique_ptr<RenderSystem> m_EngineRenderSystem; // The window the engine uses.
		std::unique_ptr<Renderer> m_EngineRenderer;

		std::unique_ptr<EngineContext> m_EngineContext; // Engine Context holds core systems that the Sandbox should be allowed to use without giving full application permissions.
		bool m_IsEngineRunning = false; // Determines of the engine is running
		void InitProps(); // Create all the systems application owns.
	};
}

