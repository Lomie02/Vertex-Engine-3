#include "pch.h"
#include "AssImpLoader.h"

#include <filesystem>

std::shared_ptr<VertexEngine::Model> VertexEngine::AssImpLoader::LoadModel(std::string path)
{
	Assimp::Importer importer; // Define the importer
	auto CompletedModel = std::make_shared<VertexEngine::Model>();

	if (!std::filesystem::exists(path.c_str())) return CompletedModel; //TODO: Add an error message here.

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

	// If the scene doesnt exist return an empty model.
	if (scene == nullptr) return std::shared_ptr<VertexEngine::Model>();



	ProcessNode(scene->mRootNode, scene, *CompletedModel);

	return CompletedModel;
}

void VertexEngine::AssImpLoader::ProcessNode(aiNode* node, const aiScene* scene, VertexEngine::Model& model)
{
	// parent meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		model.meshes.push_back(std::make_shared<VertexEngine::MeshData>(ProcessMesh(mesh)));

	}

	// Child meshes
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene, model);
	}

}

VertexEngine::MeshData VertexEngine::AssImpLoader::ProcessMesh(aiMesh* mesh)
{
	VertexEngine::MeshData finalMesh;

	// Create Vertex data

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		VertexEngine::Vertex vert;

		// Assign the positions of each vertice
		vert.position.x = mesh->mVertices[i].x;
		vert.position.y = mesh->mVertices[i].y;
		vert.position.z = mesh->mVertices[i].z;

		// Assign normals
		if (mesh->HasNormals()) {
			vert.normal.x = mesh->mNormals[i].x;
			vert.normal.y = mesh->mNormals[i].y;
			vert.normal.z = mesh->mNormals[i].z;
		}

		// Get UVs
		for (unsigned int j = 0; j < mesh->GetNumUVChannels(); j++) {
			vert.texCord.x = mesh->mTextureCoords[j][i].x;
			vert.texCord.y = mesh->mTextureCoords[j][i].y;
		}

		// tangents & Bitangents
		if (mesh->HasTangentsAndBitangents()) {

			// Get tangents first
			vert.tangent.x = mesh->mTangents[i].x;
			vert.tangent.y = mesh->mTangents[i].y;
			vert.tangent.z = mesh->mTangents[i].z;

			// Now get BiTangents
			vert.biTangent.x = mesh->mBitangents[i].x;
			vert.biTangent.y = mesh->mBitangents[i].y;
			vert.biTangent.z = mesh->mBitangents[i].z;
		}

		// Push the vertice back into the model.
		finalMesh.vertices.push_back(vert);
	}

	// Create faces of the model

	if (mesh->HasFaces()) {
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			finalMesh.indices.push_back((unsigned short)mesh->mFaces[i].mIndices[0]);
			finalMesh.indices.push_back((unsigned short)mesh->mFaces[i].mIndices[1]);
			finalMesh.indices.push_back((unsigned short)mesh->mFaces[i].mIndices[2]);
		}
	}

	//Set up basic mesh data
	finalMesh.meshName = mesh->mName.C_Str();

	// Return the final result
	return finalMesh;
}

