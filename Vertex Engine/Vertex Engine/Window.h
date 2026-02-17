#pragma once
#include <iostream>
namespace VertexEngine {
	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void SetWindowName(std::string _appName) = 0;
		virtual void SetFullscreen(bool _fullscreenWindow) = 0;

		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0; // width
		virtual unsigned int GetHeight() const = 0; // Height

		static std::unique_ptr<Window> Create();

		virtual void SetVerticalSync(bool _vSync) = 0;
		virtual bool IsWindowClosed() = 0;
		virtual void CloseWindow() = 0;

	private:
		struct WindowDataProfile {
			unsigned int Width;
			unsigned int Height;
		};

	protected:
		WindowDataProfile m_WindowData;
	};
}
