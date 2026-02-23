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
		void SetApplicationFullscreenMode(bool _fullscreenMode);
		void SetVSync(bool _vSync);
		bool IsWindowFullscreen();

		// Engine Clock Wrappers
		float GetDelta() const; // Get the current delta time in seconds
		float GetFixedDelta() const; // Get the fixed delta
		float GetFramesPerSecond() const;

		// is only here for input testing until scene manager is added.
		std::unique_ptr<InputSystem> m_EngineInputSystem;

	private:
		std::unique_ptr<EngineTime> m_EngineClock; // Allows delta to be grabbed
		std::unique_ptr<RenderSystem> m_EngineRenderSystem; // Render objects in the active scene.
		std::unique_ptr<Window> m_GameWindow; // The window the engine uses.

		std::unique_ptr<EngineContext> m_EngineContext;
		bool m_IsEngineRunning = false; // Determines of the engine is running
		void InitProps(); // Create all the systems application owns.
	};
}

