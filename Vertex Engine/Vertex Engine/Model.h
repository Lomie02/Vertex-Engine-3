#pragma once

#include "MeshData.h"
#include <memory>

namespace VertexEngine {

	class Model {
	public:
		std::string modelName;
		std::vector<std::shared_ptr<MeshData>> meshes;
	};
}