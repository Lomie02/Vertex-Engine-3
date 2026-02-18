#pragma once
#include "Renderer.h"
namespace VertexEngine {

	class RenderSystem
	{
	public:
		RenderSystem();

		void OnUpdate();
	private:
		void InitProps();
		std::unique_ptr<Renderer> m_MainlineRenderer;
	};
}

