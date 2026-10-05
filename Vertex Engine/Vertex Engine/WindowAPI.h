#pragma once
#include "Window.h"
namespace VertexEngine {
	class WindowAPI {
	public:
		explicit WindowAPI() {};
		explicit WindowAPI(VertexEngine::Window* _window);
		void SetFullscreen(bool _state); // Toggle the window to fullscreen or windowed
		bool IsFullscreen(); // Is the window fullscreen

		void SetWindowSize(unsigned int _width, unsigned int _height); // Set the windows size
		void SetVsync(bool _state); // Set vsync
	private:
		VertexEngine::Window* m_EngineWindow = nullptr; // The engines window.
	};


}