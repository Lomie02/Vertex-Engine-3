#include "pch.h"
#include "AssetManager.h"
#include <filesystem>
#include "Texture.h"
#include "Shader.h"
namespace fs = std::filesystem;
#include <iostream>
VertexEngine::AssetManager::AssetManager(std::unique_ptr<VertexEngine::MeshImporter> _importer)
{

	if (_importer) {

		m_ModelLoader = std::make_unique<VertexEngine::ModelLoader>(std::move(_importer));
	}


	// Assign the default supported types
	// Add supported image/texture types
	m_ImageTypeFilter.push_back(".png");
	m_ImageTypeFilter.push_back(".jpg");
	m_ImageTypeFilter.push_back(".JPG");

	m_ImageTypeFilter.push_back(".tga");
	m_ImageTypeFilter.push_back(".gif");
	m_ImageTypeFilter.push_back(".jpeg");

	// Add supported 3d model types
	m_MeshTypeFilter.push_back(".obj");
	m_MeshTypeFilter.push_back(".fbx");

	// Add Supported audio file types
	m_AudioTypeFilter.push_back(".wav");
	m_AudioTypeFilter.push_back(".ogg");
	m_AudioTypeFilter.push_back(".mp3");

	// Add Supported shader types
	m_ShaderTypeFilter.push_back(".vs");
	m_ShaderTypeFilter.push_back(".fs");

}

void VertexEngine::AssetManager::AutoLoadAll(std::string& _rootpath)
{
	// Load textures.
	if (fs::exists(_rootpath)) {
		for (auto& file : fs::recursive_directory_iterator(_rootpath)) {
			if (file.is_regular_file() && std::find(m_ImageTypeFilter.begin(), m_ImageTypeFilter.end(), file.path().filename().extension().string()) != m_ImageTypeFilter.end()) {
				std::string name = file.path().stem().string();
				Register<VertexEngine::Texture>(name, file.path().string());
			}
		}
	}

	// Load in models
	if (fs::exists(_rootpath) && m_ModelLoader) {
		for (auto& file : fs::recursive_directory_iterator(_rootpath)) {
			if (file.is_regular_file() && std::find(m_MeshTypeFilter.begin(), m_MeshTypeFilter.end(), file.path().filename().extension().string()) != m_MeshTypeFilter.end()) {
				std::string name = file.path().stem().string();
				Register<VertexEngine::Model>(name, file.path().string());

				std::cout << "Model: " + name + " has been loaded" << std::endl;
			}
		}
	}

	// Load in Shaders
	if (fs::exists(_rootpath)) {
		for (auto& file : fs::recursive_directory_iterator(_rootpath)) {
			if (file.is_regular_file() && std::find(m_ShaderTypeFilter.begin(), m_ShaderTypeFilter.end(), file.path().filename().extension().string()) != m_ShaderTypeFilter.end()) {
				std::string name = file.path().stem().string();
				Register<VertexEngine::Shader>(name, file.path().string());
			}
		}
	}
}

void VertexEngine::AssetManager::HotReload()
{
	//TODO: Implement hot reload 
}

void VertexEngine::AssetManager::RegisterFileType(std::string _newFileType, VertexEngine::AssetType _type)
{
	switch (_type) {
	case AssetType::image_texture:
		m_ImageTypeFilter.push_back(_newFileType);
		break;
	case AssetType::mesh_model:
		m_MeshTypeFilter.push_back(_newFileType);
		break;
	case AssetType::shader:
		m_ShaderTypeFilter.push_back(_newFileType);
		break;
	case AssetType::audio:
		m_AudioTypeFilter.push_back(_newFileType);
		break;
	}
}

void VertexEngine::AssetManager::SetRootPath(std::string _filePath, bool _reloadData)
{
	// Set the file path string
	m_AssetRootPath = _filePath;

	if (m_ModelLoader)
		m_ModelLoader->SetRootPath(m_AssetRootPath);

	// If true reload the asset files
	if (_reloadData) AutoLoadAll(m_AssetRootPath);
}
