#pragma once
#include <string>
#include <glm.hpp>

namespace VertexEngine {

	struct BoneInfo {
		std::string name;
		int32_t parentIndex = -1;
		glm::mat4 offsetMat{ 1.0f };
	};
}