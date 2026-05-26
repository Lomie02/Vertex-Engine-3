#pragma once

namespace VertexEngine {

	enum class ComponentFlags { // Used for registery for rendering.
		None = 0,
		Renderable = 1 << 0,
		Skinned = 1 << 1,
		Animated = 1 << 2
	};

	inline ComponentFlags operator|(ComponentFlags a, ComponentFlags b) {
		return static_cast<ComponentFlags>(
			static_cast<int>(a) | static_cast<int>(b)
			);
	}

	inline ComponentFlags operator&(ComponentFlags a, ComponentFlags b) {
		return static_cast<ComponentFlags>(
			static_cast<int>(a) & static_cast<int>(b)
			);
	}
}