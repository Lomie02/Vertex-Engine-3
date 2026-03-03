#pragma once
#include "EngineContex.h"
#include "Scene.h"

namespace VertexEngine {

	class GameObject;

	class Componenet {
	public:

		virtual ~Componenet() = default;
		virtual void OnEnabled() {}; // Called when script is enabled 
		virtual void OnDisabled() {}; // Called when script is disabled
		virtual void OnDestroy() {}; // called when script or gameobject is destroyed.

		virtual void OnAwake(VertexEngine::EngineContext& _engine) {}; // called on init of scene.
		virtual void OnStart(VertexEngine::EngineContext& _engine) {}; // called at the start of frame
		virtual void OnUpdate(VertexEngine::EngineContext& _engine) {}; // called every frame.

		virtual void OnLateUpdate(VertexEngine::EngineContext& _engine) {}; // called after update.
		virtual void OnFixedUpdate(VertexEngine::EngineContext& _engine) {}; // called at a fixed interval for physics steps.

		void SetEnable(bool _State)
		{
			if (!enabled && _State) OnEnabled();

			if (enabled && !_State) OnDisabled();

			enabled = _State;
		}// Enabled the script/ component, if not enabled it will be skipped in updates.
		bool IsEnabled() { return enabled; } // Is the script/component enabled for updates.

		VertexEngine::GameObject* gameObject; // Parent gameobject
	protected:
		bool enabled = true; // determines if the script is enabled & can receive updates from the scene

	private:
		bool IsStarted = false;
		unsigned int m_ComponentID; // Unique id
	};
}