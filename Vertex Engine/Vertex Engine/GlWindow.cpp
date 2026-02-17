#include "pch.h"
#include "GlWindow.h"
namespace VertexEngine {
	std::unique_ptr<Window> VertexEngine::Window::Create()
	{
		return std::make_unique<GlWindow>(1920, 1080);
	}

	GlWindow::GlWindow(unsigned int _width, unsigned int _height)
	{
		if (!glfwInit()) // Attempt to init glfw.
		{
			const char* description = new char[512];
			glfwGetError(&description);
			std::cout << "VertexError: " << description << std::endl;
		}

		// Create window & fetch the primary monitor.

		m_WindowData.Height = _height;
		m_WindowData.Width = _height;

		GLFWmonitor* currentUserPrimaryMonitor = glfwGetPrimaryMonitor();
		m_WindowHandle = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, m_WindowName.c_str(), currentUserPrimaryMonitor, nullptr);

		// Check that window was actually created.
		if (!m_WindowHandle) {
			std::cout << "VertexError: OpenGL window failed to create!" << std::endl;
		}

		glfwMakeContextCurrent(m_WindowHandle);
		std::cout << "VertexMessage: OpenGL window created." << std::endl;
	}

	GlWindow::~GlWindow()
	{
	}

	void GlWindow::SetWindowName(std::string _appName)
	{
		if (_appName.empty()) {
			std::cout << "VertexError: OpenGL window renaming failed! Empty string detected." << std::endl;
			return;
		}
		m_WindowName = _appName;

		if (!m_WindowHandle) {
			std::cout << "VertexError: OpenGL window is not initalized! Rename failed." << std::endl;
			return;
		}

		glfwSetWindowTitle(m_WindowHandle, m_WindowName.c_str());
	}

	void GlWindow::SetFullscreen(bool _fullscreenWindow = true)
	{
		// If the window handle isnt initalzed yet, print message & return to avoid crashing.
		if (!m_WindowHandle) {
			std::cout << "VertexError: OpenGL window is not initalized! Setting window mode failed." << std::endl;
			return;
		}

		// Switch between fullscreen or windowed.

		if (_fullscreenWindow) {

			// Get the current monitor alongside its video details for fullscreen.
			GLFWmonitor* currentUserPrimaryMonitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* videoMode = glfwGetVideoMode(currentUserPrimaryMonitor);
			glfwSetWindowMonitor(m_WindowHandle, currentUserPrimaryMonitor, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);

			// Update the window data so its not mismatched.
			m_WindowData.Width = videoMode->width;
			m_WindowData.Height = videoMode->height;
		}
		else {

			// Sets screen to be windowed.
			glfwSetWindowMonitor(m_WindowHandle, nullptr, 100, 100, m_WindowData.Width, m_WindowData.Height, 0);

		}
	}
	void GlWindow::OnUpdate()
	{
		glfwSwapBuffers(m_WindowHandle);
		glfwPollEvents();
	}
	unsigned int GlWindow::GetHeight() const
	{
		return m_WindowData.Height;
	}
	unsigned int GlWindow::GetWidth() const
	{
		return m_WindowData.Width;
	}
	void GlWindow::SetVerticalSync(bool _vSync)
	{
		if (m_WindowHandle)
			glfwSwapInterval(_vSync ? 1 : 0);
	}
	bool GlWindow::IsWindowClosed()
	{
		if (m_WindowHandle)
			return glfwWindowShouldClose(m_WindowHandle);

		return true; // return true, window isnt initalized so close everything.
	}
	void GlWindow::CloseWindow()
	{
		if (m_WindowHandle)
			glfwSetWindowShouldClose(m_WindowHandle, 1);
	}
}