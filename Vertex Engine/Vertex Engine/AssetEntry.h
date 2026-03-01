#pragma once
#include "Asset.h"
#include <memory>
#include <functional>
namespace VertexEngine {
	struct AssetEntry {

		std::string m_Path; // Assets location in directory
		std::function<std::shared_ptr<Asset>()> m_Factory; // Func to init asset before returning in the Get() process of the Asset manager
		std::shared_ptr<Asset> m_Instance; // The asset data itself. This is also passed through the Get() in asset manager.
	};
}