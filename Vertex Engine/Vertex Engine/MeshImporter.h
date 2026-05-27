#pragma once
#include <memory>
#include "Model.h"
namespace VertexEngine {

	class MeshImporter
	{
	public:
		virtual ~MeshImporter() {} // default deconstructor
		virtual std::shared_ptr<VertexEngine::Model> LoadModel(std::string path) = 0; // Load model from given filepath.
	};
}
