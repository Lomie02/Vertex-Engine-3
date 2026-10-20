#include "pch.h"
#include "InputAPI.h"

VertexEngine::InputAPI::InputAPI(InputSystem* _input)
{
	m_EngineInput = _input;

	if (!m_EngineInput)
		throw std::runtime_error("Vertex Error: Input API failed! Input System was bad!");
}

bool VertexEngine::InputAPI::GetKeyDown(KeyCode _key) const
{
	if (m_EngineInput)
		return m_EngineInput->GetKeyDown(_key);

	return false;
}

bool VertexEngine::InputAPI::GetKeyUp(KeyCode _key) const
{
	if (m_EngineInput)
		return m_EngineInput->GetKeyUp(_key);

	return false;
}

bool VertexEngine::InputAPI::GetKey(KeyCode _key) const
{
	if (m_EngineInput)
		return m_EngineInput->GetKey(_key);

	return false;
}

bool VertexEngine::InputAPI::GetMouse(KeyCode _key) const
{
	return false;
}

bool VertexEngine::InputAPI::GetMouseDown(KeyCode _key) const
{
	return false;
}

bool VertexEngine::InputAPI::GetMouseUP(KeyCode _key) const
{
	return false;
}

bool VertexEngine::InputAPI::GetButton(KeyCode _key, int _joystick) const
{
	if (m_EngineInput)
		return m_EngineInput->GetButton(_key, _joystick);

	return false;
}

bool VertexEngine::InputAPI::GetButtonDown(KeyCode _key, int _joystick) const
{
	return false;
}

bool VertexEngine::InputAPI::GetButtonUp(KeyCode _key, int _joystick) const
{
	return false;
}

glm::vec2 VertexEngine::InputAPI::GetAxis(std::string _axisName) const
{
	if (m_EngineInput)
		return m_EngineInput->GetAxis(_axisName);

	return glm::vec2();
}

bool VertexEngine::InputAPI::HasAxisMoved(std::string _axisName, float _deadzone) const
{
	if (m_EngineInput)
		return m_EngineInput->HasAxisMoved(_axisName, _deadzone);


	return false;
}
