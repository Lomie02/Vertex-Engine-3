#pragma once

namespace VertexEngine {

	enum class ComponentFlags { // Used for registery for rendering.
		None = 1 << 0,
		Renderable = 1 << 1,
		Skinned = 1 << 2,
		Animated = 1 << 3
	};


	inline VertexEngine::ComponentFlags operator|(VertexEngine::ComponentFlags a, VertexEngine::ComponentFlags b) {
		return static_cast<ComponentFlags>(
			static_cast<int>(a) | static_cast<int>(b)
			);
	}

	inline VertexEngine::ComponentFlags operator&(VertexEngine::ComponentFlags a, VertexEngine::ComponentFlags b) {
		return static_cast<ComponentFlags>(
			static_cast<int>(a) & static_cast<int>(b)
			);
	}
}