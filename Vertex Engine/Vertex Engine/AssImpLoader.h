#pragma once
#include "MeshImporter.h"
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MeshData.h"

namespace VertexEngine {

	class AssImpLoader : public VertexEngine::MeshImporter
	{
	public:

		std::shared_ptr<VertexEngine::Model> LoadModel(std::string path) override; // Load & return fully loaded model.

	private:
		void ProcessNode(aiNode* node, const aiScene* scene, VertexEngine::Model& model); // Processes each parent node in the models
		VertexEngine::MeshData ProcessMesh(aiMesh* mesh); // Creates & returns a mesh data container.
	};

}
