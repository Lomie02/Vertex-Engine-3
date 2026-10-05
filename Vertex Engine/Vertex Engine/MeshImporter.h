#pragma once
#include <memory>
#include "Model.h"
namespace VertexEngine {

	class MeshImporter
	{
	public:
		virtual ~MeshImporter() {} // default deconstructor
		virtual std::shared_ptr<VertexEngine::Model> LoadModel(std::string path) = 0; // Load model from given filepath.
		virtual void SetRootPath(std::string rootPath) = 0; // Set the root path to look inside

	protected:
		std::string m_RootPath = "assets";
	};
}
