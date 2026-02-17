#include "pch.h"
#include "EngineTime.h"
#include "glfw3.h"

namespace VertexEngine {
	EngineTime::EngineTime()
	{
		m_Frames = 0;
		m_UnscaledTime = 0;
		m_fpsInterval = 0;
		m_PrevTime = (float)glfwGetTime();
	}

	void EngineTime::ConfigureDeltaTime()
	{
		double currentTime = (float)glfwGetTime();
		m_UnscaledTime = currentTime - m_PrevTime;
		m_PrevTime = currentTime;

		m_Frames++;
		m_fpsInterval += m_UnscaledTime;

		if (m_fpsInterval > 1.0f) {
			m_Frames = 0;
			m_fpsInterval -= 1.0f;
		}

		m_FixedDelta += m_UnscaledTime * m_TimeScale;
		m_DeltaTime = m_UnscaledTime * m_TimeScale;
	}

	float VertexEngine::EngineTime::GetDeltaTime()
	{
		return m_DeltaTime;
	}

	float VertexEngine::EngineTime::GetFixedDeltaTime()
	{
		return m_FixedDelta;
	}

	void VertexEngine::EngineTime::SetTimeScale(float _scaleTime)
	{
		m_TimeScale = _scaleTime;
	}

	float EngineTime::GetTimeStep()
	{
		return m_TimeStep;
	}

	void EngineTime::ResetFixedDelta()
	{
		m_FixedDelta -= m_TimeStep;
	}

}