#pragma once
#include "Window.h"
namespace VertexEngine {
	class WindowAPI {
	public:
		explicit WindowAPI(VertexEngine::Window* _window);
		void SetFullscreen(bool _state); // Toggle the window to fullscreen or windowed
		bool IsFullscreen(); // Is the window fullscreen

		void SetVsync(bool _state); // Set vsync
	private:
		VertexEngine::Window* m_EngineWindow; // The engines window.
	};


}