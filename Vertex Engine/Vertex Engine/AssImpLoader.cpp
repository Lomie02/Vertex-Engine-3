#include "pch.h"
#include "AssImpLoader.h"
#include <iostream>
#include <filesystem>
#include "ModelNode.h"
#include <ext/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

#include <algorithm>
#include <cctype>

std::shared_ptr<VertexEngine::Model> VertexEngine::AssImpLoader::LoadModel(std::string path)
{
	Assimp::Importer importer; // Define the importer
	auto CompletedModel = std::make_shared<VertexEngine::Model>();

	if (!std::filesystem::exists(m_RootPath + path.c_str() + ".fbx")) { std::cout << "Failed to load model at " << std::endl; return CompletedModel; } //TODO: Add an error message here.

	const aiScene* scene = importer.ReadFile(m_RootPath + path.c_str() + ".fbx", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

	// If the scene doesnt exist return an empty model.   
	if (scene == nullptr) return std::shared_ptr<VertexEngine::Model>();

	constexpr float CentimetersToMeters = 0.01f;

	float unitScale = CentimetersToMeters;

	if (scene->mMetaData) {
		ai_real unitScaleFactor = 100.0f;

		if (scene->mMetaData->Get("UnitScaleFactor", unitScaleFactor)) {
			unitScale = static_cast<float>(unitScaleFactor) * CentimetersToMeters;
		}
	}

	CompletedModel->modelName = path;

	// Extract materials for the final model
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		MaterialData data = ProcessMaterial(material);

		CompletedModel->materialData.push_back(data);
	}

	ProcessNode(scene->mRootNode, scene, CompletedModel, CompletedModel->m_RootNode, unitScale);

	return CompletedModel;
}

void VertexEngine::AssImpLoader::SetRootPath(std::string rootPath)
{
	m_RootPath = rootPath;
}

void VertexEngine::AssImpLoader::ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<VertexEngine::Model> model, VertexEngine::ModelNode& modelNode, float _unitScale)
{
	modelNode.m_Name = node->mName.C_Str();
	modelNode.m_LocalTransform = ConvertNodeTransform(node->mTransformation, _unitScale, (node == scene->mRootNode));

	// parent meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		uint32_t meshIndex = static_cast<uint32_t>(model->meshes.size());

		model->meshes.push_back(std::make_shared<VertexEngine::MeshData>(ProcessMesh(mesh)));

		modelNode.m_MeshIndices.push_back(meshIndex);
	}

	// Child meshes
	for (unsigned int i = 0; i < node->mNumChildren; i++) {

		modelNode.m_Children.emplace_back();
		ProcessNode(node->mChildren[i], scene, model, modelNode.m_Children.back(), _unitScale);
	}
}

VertexEngine::MeshData VertexEngine::AssImpLoader::ProcessMesh(aiMesh* mesh)
{
	VertexEngine::MeshData finalMesh;

	// Create Vertex data

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		VertexEngine::Vertex vert{};

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
			vert.texCord.x = mesh->mTextureCoords[0][i].x;
			vert.texCord.y = mesh->mTextureCoords[0][i].y;
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

	VertexEngine::SubMeshData subMesh;

	subMesh.indexOffset = 0;
	subMesh.indexCount = static_cast<uint32_t>(finalMesh.indices.size());
	subMesh.materialIndex = mesh->mMaterialIndex;

	finalMesh.subMeshes.push_back(subMesh);

	return finalMesh;
}

glm::mat4 VertexEngine::AssImpLoader::ConvertMatrix(const aiMatrix4x4& _matrix)
{
	glm::mat4 result{ 1.0f };

	result[0][0] = _matrix.a1;
	result[0][1] = _matrix.b1;
	result[0][2] = _matrix.c1;
	result[0][3] = _matrix.d1;

	result[1][0] = _matrix.a2;
	result[1][1] = _matrix.b2;
	result[1][2] = _matrix.c2;
	result[1][3] = _matrix.d2;

	result[2][0] = _matrix.a3;
	result[2][1] = _matrix.b3;
	result[2][2] = _matrix.c3;
	result[2][3] = _matrix.d3;

	result[3][0] = _matrix.a4;
	result[3][1] = _matrix.b4;
	result[3][2] = _matrix.c4;
	result[3][3] = _matrix.d4;

	return result;
}

glm::mat4 VertexEngine::AssImpLoader::ConvertNodeTransform(const aiMatrix4x4& _matrix, float _unitScale, bool _isRoot)
{
	aiVector3D scaling;
	aiQuaternion rotation;
	aiVector3D position;

	_matrix.Decompose(
		scaling,
		rotation,
		position
	);

	// FBX/Assimp units -> engine units
	position *= _unitScale;

	glm::vec3 glmPosition(
		position.x,
		position.y,
		position.z
	);

	glm::vec3 glmScale(scaling.x, scaling.y, scaling.z);

	if (!_isRoot) {
		glmScale *= _unitScale;
	}

	glm::quat glmRotation(
		rotation.w,
		rotation.x,
		rotation.y,
		rotation.z
	);

	return glm::translate(glm::mat4(1.0f), glmPosition)
		* glm::mat4_cast(glmRotation) * glm::scale(glm::mat4(1.0f), glmScale);
}

std::string VertexEngine::AssImpLoader::ExtractRawTexture(aiMaterial* _material, const char* propertie)
{
	for (unsigned int i = 0; i < _material->mNumProperties; i++) {

		aiMaterialProperty* prop = _material->mProperties[i];

		if (std::string(prop->mKey.C_Str()) == propertie) {

			if (prop->mDataLength <= 1)
				return "";

			const char* data = reinterpret_cast<const char*>(prop->mData);

			std::string path(data, prop->mDataLength);

			if (!path.empty()) path.erase(0, 1);

			return path;
		}
	}


	return std::string();
}

VertexEngine::MaterialData VertexEngine::AssImpLoader::ProcessMaterial(aiMaterial* _material)
{

	VertexEngine::MaterialData data;

	// Get name of material
	aiString name;
	if (_material->Get(AI_MATKEY_NAME, name) == AI_SUCCESS) {
		data.m_Name = name.C_Str();
	}

	// Albedo

	aiColor4D colour;

	// Try modern PBR base color first.
	if (_material->Get(AI_MATKEY_BASE_COLOR, colour) == AI_SUCCESS)
	{
		data.m_AlbedoColour = glm::vec4(
			colour.r,
			colour.g,
			colour.b,
			colour.a
		);
	}
	// Fall back to the traditional diffuse colour.
	else if (_material->Get(AI_MATKEY_COLOR_DIFFUSE, colour) == AI_SUCCESS)
	{
		data.m_AlbedoColour = glm::vec4(
			colour.r,
			colour.g,
			colour.b,
			colour.a
		);
	}

	data.m_AlbedoMap = FetchFirstAbledo(_material);

	if (!data.m_AlbedoMap.empty())
	{
		std::cout
			<< "Selected Albedo: "
			<< data.m_AlbedoMap
			<< "\n";
	}
	else
	{
		std::cout
			<< "No Albedo Texture Found for: "
			<< data.m_Name
			<< "\n";
	}

	// ============================ Set default metallic & roughness values or fetch them from export

	float metallic = 0.0f;
	float roughness = 1.0f;

	if (_material->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS) {
		data.m_Metallic = metallic;
	}

	if (_material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS) {
		data.m_Roughness = roughness;
	}

	/*if (data.m_Name == "Dva_Hair1")
	{
		data.m_AlbedoOverride = "DVa_Hair_D_Classic.jpg";
	}*/

	return data;
}

std::string VertexEngine::AssImpLoader::FetchFirstAbledo(aiMaterial* _material)
{

	if (_material->GetTextureCount(aiTextureType_BASE_COLOR) > 0) {

		aiString path;

		if (_material->GetTexture(aiTextureType_BASE_COLOR, 0, &path) == AI_SUCCESS) {
			return path.C_Str();
		}
	}


	if (_material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {

		aiString path;

		if (_material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
			return path.C_Str();
		}
	}

	for (unsigned int i = 0; i < _material->mNumProperties; i++)
	{
		aiMaterialProperty* prop = _material->mProperties[i];

		std::string key = prop->mKey.C_Str();

		if (key == "$tex.file")
		{
			const char* raw =
				reinterpret_cast<const char*>(prop->mData);

			std::string path(raw, prop->mDataLength);

			std::cout
				<< "RAW TEX FILE: ["
				<< path
				<< "]\n";
		}
	}

	return {};
}

std::string VertexEngine::AssImpLoader::FetchFirstTexturePath(aiMaterial* _material)
{
	for (unsigned int i = 0; i < _material->mNumProperties; i++)
	{
		aiMaterialProperty* prop = _material->mProperties[i];

		if (prop->mType != aiPTI_String)
			continue;

		const char* raw =
			reinterpret_cast<const char*>(prop->mData);

		std::string value(raw, prop->mDataLength);

		// Remove the weird leading type/length character.
		if (!value.empty() && !std::isalpha(value[0]) &&
			value[0] != '\\' && value[0] != '/')
		{
			value.erase(0, 1);
		}

		std::string lower = value;

		std::transform(
			lower.begin(),
			lower.end(),
			lower.begin(),
			[](unsigned char c)
			{
				return static_cast<char>(std::tolower(c));
			});

		if (lower.find(".jpg") != std::string::npos ||
			lower.find(".jpeg") != std::string::npos ||
			lower.find(".png") != std::string::npos ||
			lower.find(".tga") != std::string::npos ||
			lower.find(".bmp") != std::string::npos ||
			lower.find(".dds") != std::string::npos)
		{
			std::cout
				<< "Texture Candidate: "
				<< value
				<< "\n";

			return value;
		}
	}

	return {};
}

