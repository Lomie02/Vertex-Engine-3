#include "pch.h"
#include "InputAPI.h"

VertexEngine::InputAPI::InputAPI(InputSystem* _input)
{
	m_EngineInput = _input;
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
