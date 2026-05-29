#pragma once
#include "AssetEntry.h"
#include <vector>
#include <string>
#include "AssetType.h"
#include "ModelLoader.h"
namespace VertexEngine {

	class AssetManager
	{
	public:
		AssetManager(std::unique_ptr<VertexEngine::MeshImporter> _importer);

		// Automatically load all assets in the given path.
		void AutoLoadAll(std::string& _rootpath);
		void HotReload(); // Reloads the asset database.

		// Register a asset manually.
		template<typename T>
		void Register(const std::string& _name, const std::string& _filepath, bool useModelList = false) {
			AssetEntry newEntry;
			newEntry.m_Path = _filepath;
			newEntry.m_Factory = []() { return std::make_shared<T>(); };

			if (!useModelList)
				m_AssetList[_name] = std::move(newEntry);
			else
				m_ModelList[_name] = std::move(newEntry);
		}

		// Get requested asset
		template<typename T>
		std::shared_ptr<T> Get(const std::string& _name) {

			bool isModel = false;

			// Check the normal asset list first.
			auto it = m_AssetList.find(_name);
			if (it == m_AssetList.end()) {
				isModel = true; // if asset is not in the main asset it might be a model.
			}

			// Check for model
			if (isModel) {

				auto it = m_ModelList.find(_name);
				if (it == m_ModelList.end()) {
					// If nothing is found then the asset doesnt exist at all.
					return nullptr;
				}


				auto& ent = m_ModelList.at(_name);

				return std::dynamic_pointer_cast<T>(m_ModelLoader->LoadModel(_name));
			}


			// Look through the normal asset list if not a model.
			auto& ent = m_AssetList.at(_name);

			if (!ent.m_Instance) {
				ent.m_Instance = ent.m_Factory();
				ent.m_Instance->Load(ent.m_Path);  
			}

			return std::dynamic_pointer_cast<T>(ent.m_Instance);
		}

		void RegisterFileType(std::string _newFileType, VertexEngine::AssetType _type); // Add custom or more file types.
		void SetRootPath(std::string _filePath, bool _reloadData = false); // Set the file path the engine should load assets from.

	private:
		std::unique_ptr<VertexEngine::ModelLoader> m_ModelLoader;

		std::unordered_map<std::string, AssetEntry> m_AssetList; // all assets are not loaded unless they are needed. Only references are saved.
		std::unordered_map<std::string, AssetEntry> m_ModelList; // all models that are loaded.
		std::vector<std::string> m_ImageTypeFilter; // All supported file types that the engine should filter.
		std::vector<std::string> m_MeshTypeFilter; // All supported file types that the engine should filter.
		std::vector<std::string> m_AudioTypeFilter; // All supported file types that the engine should filter.
		std::vector<std::string> m_ShaderTypeFilter; // All supported file types that the engine should filter.
		std::string m_AssetRootPath = "asset"; // The file path the engine should use

		bool m_HasStarted = false;
	};
}

