#pragma once
#include "EngineTime.h"
namespace VertexEngine {
	class TimeAPI {
	public:
		explicit TimeAPI(VertexEngine::EngineTime* _time);
		float GetDeltaTime(); // Get the delta time
		float GetFixedDeltaTime(); // Get delta in fixed intervals, used for fixed update

		void SetTimeScale(float _scale); // Set the time scale to slow or speed up time
		float GetTimeScale(); // Returns the current time scale

	private:
		VertexEngine::EngineTime* m_EngineTime; // The engines time
	};
}