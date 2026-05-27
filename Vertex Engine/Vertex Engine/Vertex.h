#pragma once
#include <glm.hpp>
namespace VertexEngine {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 texCord;
		glm::vec3 tangent;
		glm::vec3 biTangent;
	};
}