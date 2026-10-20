#pragma once
#include "InputSystem.h"
#include <glm.hpp>
namespace VertexEngine {

	class InputAPI {
	public:
		explicit InputAPI() {};
		explicit InputAPI(InputSystem* _input);

		// Keyboard inputs
		bool GetKeyDown(KeyCode _key) const; // Returns true if the key is pressed once
		bool GetKeyUp(KeyCode _key) const; // Returns true if the key isnt pressed
		bool GetKey(KeyCode _key) const; // Returns true if key is being held down

		// Mouse Inputs
		bool GetMouse(KeyCode _key) const;
		bool GetMouseDown(KeyCode _key) const;
		bool GetMouseUP(KeyCode _key) const;

		// Gamepad Inputs
		bool GetButton(KeyCode _key, int _joystick) const;
		bool GetButtonDown(KeyCode _key, int _joystick) const;
		bool GetButtonUp(KeyCode _key, int _joystick) const;

		glm::vec2 GetAxis(std::string _axisName) const;
		bool HasAxisMoved(std::string _axisName, float _deadzone) const;

	private:
		InputSystem* m_EngineInput = nullptr; // ptr is a ref to the engine owned input system
	};
}