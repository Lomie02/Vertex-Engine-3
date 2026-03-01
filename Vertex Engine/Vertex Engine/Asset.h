#pragma once
#include <string>
namespace VertexEngine {
	class Asset {
	public:
		virtual ~Asset() = default;
		virtual void Load(const std::string& _path) = 0; // Load the asset from disk or directory
		bool IsLoaded() const { return m_AssetLoaded; } // Has the requested asset been loaded in the engine.

	protected:
		// Asset load status.
		bool m_AssetLoaded = false;
	};
}