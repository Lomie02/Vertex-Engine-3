#pragma once
#include "Window.h"
#include "glfw3.h"
namespace VertexEngine {
	class GlWindow : public VertexEngine::Window
	{
	public:

		GlWindow(unsigned int _width, unsigned int _height);

		~GlWindow() override;
		void SetWindowName(std::string _appName) override;
		void SetFullscreen(bool _fullscreenWindow) override;
		void OnUpdate() override;

		unsigned int GetHeight() const override;
		unsigned int GetWidth() const override;
		void SetVerticalSync(bool _vSync) override;
		bool IsWindowClosed() override;
		void CloseWindow() override;
	private:
		GLFWwindow* m_WindowHandle = nullptr;
		std::string m_WindowName = "Vertex Engine 3";
	};
}

