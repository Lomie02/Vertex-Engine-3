#pragma once
/*
	Application for running
*/
#include "Window.h"
#include <iostream>
#include "EngineTime.h"

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

		// Engine Clock Wrappers
		float GetDelta() const; // Get the current delta time in seconds
		float GetFixedDelta() const; // Get the fixed delta


	private:
		std::unique_ptr<VertexEngine::EngineTime> m_EngineClock;
		bool m_IsEngineRunning = false; // Determines of the engine is running
		std::unique_ptr<Window> m_GameWindow;
		void InitProps();
	};
}

