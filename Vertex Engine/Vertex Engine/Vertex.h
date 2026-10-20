#pragma once
#include <glm.hpp>
namespace VertexEngine {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCord;
		glm::vec3 tangent;
		glm::vec3 biTangent;

		// Skeletal
		glm::ivec4 boneIDs = glm::ivec4(-1);
		glm::vec4 boneWeights = glm::vec4(0.0f);
	}; 
}