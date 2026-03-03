#pragma once

namespace VertexEngine {

	class EngineTime
	{
	public:
		EngineTime();
		void ConfigureDeltaTime(); // Calculate delta & fixed times

		float GetDeltaTime(); // Return the current delta time
		float GetFixedDeltaTime(); // Return the current fixed delta

		void SetTimeScale(float _scaleTime); // Set the speed that time passes
		float GetTimeScale(); // Get the current time scale

		float GetTimeStep(); // Get the current time step
		void ResetFixedDelta(); // Reset fixed delta for next caclulation loop.

		float GetAppFramesPerSecond() const; // returns the amount of frames passed per second.

	private:
		float m_FixedDelta;
		float m_DeltaTime;
		float m_TimeScale = 1;
		float m_UnscaledTime;

		float m_Frames;
		float m_fpsInterval;
		float m_PrevTime;
		float m_TimeStep = 0.01f;

		float m_FramesPerSecond;
	};
}

