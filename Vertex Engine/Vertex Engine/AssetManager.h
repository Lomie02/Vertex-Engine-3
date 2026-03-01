#pragma once
#include "AssetEntry.h"
namespace VertexEngine {

	class AssetManager
	{
	public:

		// Automatically load all assets in the given path.
		void AutoLoadAll(std::string& _rootpath);

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

	private:
		std::unordered_map<std::string, AssetEntry> m_AssetList; // all assets are not loaded unless theyred needed. Only references are saved.
	};
}

