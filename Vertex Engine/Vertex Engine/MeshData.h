#pragma once
#include <string>
#include <vector>
#include "SubMeshData.h"
#include "Vertex.h"
namespace VertexEngine {

	struct MeshData {
		std::string meshName;
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		std::vector<VertexEngine::SubMeshData> subMeshes;
		bool m_IsUploaded = false;
		uint32_t m_gpuId = 0;
	};
}