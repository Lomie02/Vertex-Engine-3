#pragma once
#include "AssetEntry.h"
#include <vector>
#include <string>
#include "AssetType.h"
namespace VertexEngine {

	class AssetManager
	{
	public:
		AssetManager();

		// Automatically load all assets in the given path.
		void AutoLoadAll(std::string& _rootpath);
		void HotReload(); // Reloads the asset database.

		// Register a asset manually.
		template<typename T>
		void Register(const std::string& _name, const std::string& _filepath) {
			AssetEntry newEntry;
			newEntry.m_Path = _filepath;
			newEntry.m_Factory = []() { return std::make_shared<T>(); };

			m_AssetList[_name] = std::move(newEntry);
		}

		// Get requested asset
		template<typename T>
		std::shared_ptr<T> Get(const std::string& _name) {

			auto it = m_AssetList.find(_name);
			if (it == m_AssetList.end()) {
				return nullptr;
			}

			auto& ent = m_AssetList.at(_name);

			if (!ent.m_Instance) {
				ent.m_Instance = ent.m_Factory();
				ent.m_Instance->Load(ent.m_Path);
			}

			return std::dynamic_pointer_cast<T>(ent.m_Instance);
		}

		void RegisterFileType(std::string _newFileType,  VertexEngine::AssetType _type); // Add custom or more file types.
		void SetRootPath(std::string _filePath, bool _reloadData = false); // Set the file path the engine should load assets from.

	private:


		std::unordered_map<std::string, AssetEntry> m_AssetList; // all assets are not loaded unless theyred needed. Only references are saved.
		std::vector<std::string> m_ImageTypeFilter; // All supported file types that the engine should filter.
		std::vector<std::string> m_MeshTypeFilter; // All supported file types that the engine should filter.
		std::vector<std::string> m_AudioTypeFilter; // All supported file types that the engine should filter.
		std::vector<std::string> m_ShaderTypeFilter; // All supported file types that the engine should filter.
		std::string m_AssetRootPath = "asset"; // The file path the engine should use

		bool m_HasStarted = false;
	};
}

