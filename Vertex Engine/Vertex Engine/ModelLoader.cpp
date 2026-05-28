#include "pch.h"
#include "ModelLoader.h"

std::shared_ptr<VertexEngine::Model> VertexEngine::ModelLoader::LoadModel(std::string _filepath)
{
	if (!m_MeshImporter) return std::shared_ptr<VertexEngine::Model>();

	return m_MeshImporter->LoadModel(_filepath);
}

void VertexEngine::ModelLoader::SetRootPath(std::string _rootPath)
{
	if (m_MeshImporter)
		m_MeshImporter->SetRootPath(_rootPath);
}
