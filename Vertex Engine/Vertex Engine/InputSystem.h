#pragma once
#include "Window.h"
#include "KeyCode.h"
namespace VertexEngine {

	class InputSystem
	{
	public:
		virtual ~InputSystem() = default;
		virtual bool GetKeyDown(KeyCode _key) = 0;
		virtual bool GetKeyUp(KeyCode _key) = 0;
		virtual bool GetKey(KeyCode _key) = 0;

		// Mouse Inputs
		virtual bool GetMouse(KeyCode _key) = 0;
		virtual bool GetMouseDown(KeyCode _key) = 0;
		virtual bool GetMouseUP(KeyCode _key) = 0;

		// Gamepad Inputs
		virtual bool GetButton(KeyCode _key, int _joystick) = 0;
		virtual bool GetButtonDown(KeyCode _key, int _joystick) = 0;
		virtual bool GetButtonUp(KeyCode _key, int _joystick) = 0;

		virtual bool IsGamepad(int _joystick) = 0;

		// Engine access functions
		virtual void UpdateInputLogs() = 0;
	};
}
