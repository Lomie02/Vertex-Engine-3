#include "pch.h"
#include "AssetManager.h"
#include <filesystem>
#include "Texture.h"
#include "Shader.h"
namespace fs = std::filesystem;

void VertexEngine::AssetManager::AutoLoadAll(std::string& _rootpath)
{
	// Load textures.
	if (fs::exists(_rootpath + "/Textures")) {
		for (auto& file : fs::directory_iterator(_rootpath + "/Textures")) {
			std::string name = file.path().stem().string();
			Register<VertexEngine::Texture>(name, file.path().string());
		}
	}

	// Load in models
	if (fs::exists(_rootpath + "/Models")) {
		for (auto& file : fs::directory_iterator(_rootpath + "/Models")) {
			std::string name = file.path().stem().string();
			Register<VertexEngine::Texture>(name, file.path().string());
		}
	}

	// Load in Shaders
	if (fs::exists(_rootpath + "/Shaders")) {
		for (auto& file : fs::directory_iterator(_rootpath + "/Shaders")) {
			std::string name = file.path().stem().string();
			Register<VertexEngine::Shader>(name, file.path().string());
		}
	}


}
