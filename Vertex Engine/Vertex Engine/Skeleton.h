#pragma once
#include <vector>
#include "BoneInfo.h"
namespace VertexEngine {

	struct Skeleton {
		std::vector<BoneInfo> m_bones;
	};
}