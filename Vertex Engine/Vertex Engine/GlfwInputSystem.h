#pragma once
#include "InputSystem.h"
#include "GlWindow.h"
#include "glfw3.h"

namespace VertexEngine {

	class GlfwInputSystem : public VertexEngine::InputSystem
	{
	public:
		GlfwInputSystem(VertexEngine::GlWindow* _window);

		// Keyboard Inputs
		virtual bool GetKey(KeyCode _key)  override;
		virtual bool GetKeyUp(KeyCode _key)  override;
		virtual bool GetKeyDown(KeyCode _key)  override;

		// Mouse Inputs
		virtual bool GetMouse(KeyCode _key)override;
		virtual bool GetMouseDown(KeyCode _key) override;
		virtual bool GetMouseUP(KeyCode _key) override;

		// Gamepad Inputs
		virtual bool GetButton(KeyCode _key, int _joystick) override;
		virtual bool GetButtonDown(KeyCode _key, int _joystick) override;
		virtual bool GetButtonUp(KeyCode _key, int _joystick) override;

		virtual bool IsGamepad(int _joystick) override;

		// Engine Functions
		virtual void UpdateInputLogs() override; // Updates all input
		int ConvertKeycodeToGLFW(KeyCode _keycode); // Convert the native engine keycodes into GLFW for input processing

	private:
		void InitProps(); // assign maps & arrays.

		VertexEngine::GlWindow* m_GameWindow; // GLFW Wrapped window
		bool m_CurrentKeys[GLFW_KEY_LAST]; // All current keyboard inputs.
		bool m_PreviousKeys[GLFW_KEY_LAST]; // All previous keyboard inputs.

		bool m_CurrentMouseKeys[GLFW_MOUSE_BUTTON_LAST]; // All current mouse inputs
		bool m_PreviousMouseKeys[GLFW_MOUSE_BUTTON_LAST]; // All previous mouse inputs

		// 15 is used for the array size for the limited number of buttons a controller has. so 15 is enough.
		bool m_CurrentGamepadKeys[15]; // All current gamepad inputs
		bool m_PreviousGamepadKeys[15]; // All previous gamepad inputs

	};
}

