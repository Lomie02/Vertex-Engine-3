#pragma once

#include "MeshData.h"
#include <memory>
#include "Asset.h"
#include <string>
#include <vector>
#include "ModelNode.h"
#include "Material.h"
#include "Skeleton.h"
namespace VertexEngine {

	class Model : public VertexEngine::Asset{
	public:
		std::string modelName = "untitled";
		std::vector<std::shared_ptr<MeshData>> meshes;
		void Load(const std::string& _path) override {}; // required here but is not used.
	
		std::vector<Material> materials;
		std::vector<MaterialData> materialData;
		std::shared_ptr<Skeleton> m_Skeleton;

		ModelNode m_RootNode;
	};
}