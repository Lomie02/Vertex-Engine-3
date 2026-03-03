#pragma once
#include "InputAPI.h"
#include "WindowAPI.h"
#include "TimeAPI.h"
// The EngineContex allows sandbox componets/ VertexBehaviour scripts to interact with some systems from the engine with an API wrapper.
// wrapping core systems in a sandbox api allows the user to still use input, window settings, time & more without having access to the engine code directly.
// There is only 1 engine context that is owned by the engine.

namespace VertexEngine {

	struct EngineContext {
		std::unique_ptr<InputAPI> Input; // Sandbox API for Input
		std::unique_ptr<WindowAPI> Window; // Sandbox API for Window
		std::unique_ptr<TimeAPI> Time; // Sandbox API for Engine Time
	};
}