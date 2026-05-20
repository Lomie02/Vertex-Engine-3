#include "pch.h"
#include "TimeAPI.h"
#include <iostream>

VertexEngine::TimeAPI::TimeAPI(VertexEngine::EngineTime* _time)
{
	m_EngineTime = _time;

	if (!m_EngineTime)
		throw std::runtime_error("Vertex Error: Time API failed! Engine Time was bad.");
}

float VertexEngine::TimeAPI::GetDeltaTime()
{
	if (m_EngineTime)
		return m_EngineTime->GetDeltaTime();

	return 0.0f;
}

float VertexEngine::TimeAPI::GetFixedDeltaTime()
{
	if (m_EngineTime)
		m_EngineTime->GetFixedDeltaTime();

	return 0.0f;
}

void VertexEngine::TimeAPI::SetTimeScale(float _scale)
{
	if (m_EngineTime)
		m_EngineTime->SetTimeScale(_scale);
}

float VertexEngine::TimeAPI::GetTimeScale()
{
	if (m_EngineTime)
		m_EngineTime->GetTimeScale();

	return 0.0f;
}
