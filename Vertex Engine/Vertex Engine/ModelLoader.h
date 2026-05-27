#pragma once
#include "MeshImporter.h"
#include <memory>
#include "Model.h"
#include <string>
namespace VertexEngine {

	class ModelLoader
	{
	public:
		ModelLoader(VertexEngine::MeshImporter* _loader); 
		std::shared_ptr<VertexEngine::Model> LoadModel(std::string _filepath); // Loads & returns given model

	private:
		MeshImporter* m_MeshImporter; // The importer for loading meshes.
	};

}
