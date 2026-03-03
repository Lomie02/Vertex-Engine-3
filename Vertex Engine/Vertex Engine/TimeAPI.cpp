#include "pch.h"
#include "TimeAPI.h"

VertexEngine::TimeAPI::TimeAPI(VertexEngine::EngineTime* _time)
{
	m_EngineTime = _time;
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
