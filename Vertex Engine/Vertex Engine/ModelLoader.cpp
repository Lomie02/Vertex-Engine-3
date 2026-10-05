#include "pch.h"
#include "ModelLoader.h"
#include <iostream>

std::shared_ptr<VertexEngine::Model> VertexEngine::ModelLoader::LoadModel(std::string _filepath)
{
	if (!m_MeshImporter) { std::cout << "Failed to load model" << std::endl; return std::shared_ptr<VertexEngine::Model>(); }

	return m_MeshImporter->LoadModel(_filepath);
}

void VertexEngine::ModelLoader::SetRootPath(std::string _rootPath)
{
	if (m_MeshImporter)
		m_MeshImporter->SetRootPath(_rootPath);
}
