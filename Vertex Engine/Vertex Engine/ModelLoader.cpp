#include "pch.h"
#include "ModelLoader.h"

VertexEngine::ModelLoader::ModelLoader(VertexEngine::MeshImporter* _loader)
{
	m_MeshImporter = _loader;
}

std::shared_ptr<VertexEngine::Model> VertexEngine::ModelLoader::LoadModel(std::string _filepath)
{
	if (!m_MeshImporter) return std::shared_ptr<VertexEngine::Model>();

	return m_MeshImporter->LoadModel(_filepath);
}
