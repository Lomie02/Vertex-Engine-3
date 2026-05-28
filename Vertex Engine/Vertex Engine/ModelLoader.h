#pragma once
#include "MeshImporter.h"
#include <memory>
#include "Model.h"
#include <string>
namespace VertexEngine {

	class ModelLoader
	{
	public:
		ModelLoader(std::unique_ptr<VertexEngine::MeshImporter> _loader) : m_MeshImporter(std::move(_loader)){}
		std::shared_ptr<VertexEngine::Model> LoadModel(std::string _filepath); // Loads & returns given model
		void SetRootPath(std::string _rootPath); // Set the root path to look for models

	private:
		std::unique_ptr<VertexEngine::MeshImporter> m_MeshImporter; // The importer for loading meshes.
	};

}
