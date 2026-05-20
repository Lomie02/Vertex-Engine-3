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
