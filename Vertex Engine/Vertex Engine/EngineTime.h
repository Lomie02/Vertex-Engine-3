#pragma once

namespace VertexEngine {

	class EngineTime
	{
	public:
		EngineTime();
		void ConfigureDeltaTime();

		float GetDeltaTime();
		float GetFixedDeltaTime();
		void SetTimeScale(float _scaleTime);
		float GetTimeStep();
		void ResetFixedDelta();

	private:
		float m_FixedDelta;
		float m_DeltaTime;
		float m_TimeScale = 1;
		float m_UnscaledTime;

		float m_Frames;
		float m_fpsInterval;
		float m_PrevTime;
		float m_TimeStep = 0.01f;
	};
}

