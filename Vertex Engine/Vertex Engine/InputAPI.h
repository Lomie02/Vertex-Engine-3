#pragma once
#include "InputSystem.h"
namespace VertexEngine {

	class InputAPI {
	public:
		explicit InputAPI(InputSystem* _input);

		// Keyboard inputs
		bool GetKeyDown(KeyCode _key) const;
		bool GetKeyUp(KeyCode _key) const;
		bool GetKey(KeyCode _key) const;

		// Mouse Inputs
		bool GetMouse(KeyCode _key) const;
		bool GetMouseDown(KeyCode _key) const;
		bool GetMouseUP(KeyCode _key) const;

		// Gamepad Inputs
		bool GetButton(KeyCode _key, int _joystick) const;
		bool GetButtonDown(KeyCode _key, int _joystick) const;
		bool GetButtonUp(KeyCode _key, int _joystick) const;
	private:
		InputSystem* m_EngineInput;
	};
}