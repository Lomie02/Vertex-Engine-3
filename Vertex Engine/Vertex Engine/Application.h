#pragma once

namespace VertexEngine {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Execute(); // Run the engine

	protected:
		virtual void OnUpdate() {} // Called Every frame
		bool m_IsEngineRunning = false;
	};
}

