#pragma once
#include <string>
#include <glm.hpp>
#include <vector>
namespace VertexEngine{
	struct ModelNode {
		std::string m_Name;

		glm::mat4 m_LocalTransform{1.0f};

		std::vector<uint32_t> m_MeshIndices;
		std::vector<ModelNode> m_Children;
	};
}