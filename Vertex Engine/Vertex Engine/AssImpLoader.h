#pragma once
#include "MeshImporter.h"
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MeshData.h"
#include <assimp/material.h>
#include "MaterialData.h"

namespace VertexEngine {

	class AssImpLoader : public VertexEngine::MeshImporter
	{
	public:

		std::shared_ptr<VertexEngine::Model> LoadModel(std::string path) override; // Load & return fully loaded model.
		void SetRootPath(std::string rootPath) override;
	private:
		void ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<VertexEngine::Model> model, VertexEngine::ModelNode& modelNode, float _unitScale); // Processes each parent node in the models
		VertexEngine::MeshData ProcessMesh(aiMesh* mesh); // Creates & returns a mesh data container.
		glm::mat4 ConvertMatrix(const aiMatrix4x4& _matrix);
		glm::mat4 ConvertNodeTransform(const aiMatrix4x4& _matrix, float _unitScale, bool _isRoot);

		static std::string ExtractRawTexture(aiMaterial* _material, const char* propertie);
		MaterialData ProcessMaterial(aiMaterial* _material);
		void OverrideMaterialAlbedo(const std::string& _materialName, const std::string& _textureName);

		std::string FetchFirstAbledo(aiMaterial* _material);
		std::string FetchFirstTexturePath(aiMaterial* _material);

		float m_UnitScale = 0.01f;
	};

}
