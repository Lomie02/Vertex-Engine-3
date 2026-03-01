#pragma once
#include "Renderer.h"
namespace VertexEngine {

	class RenderSystem
	{
	public:
		RenderSystem(Renderer* _renderAPI);

		void OnUpdate();
	private:
		void InitProps();
		Renderer* m_MainlineRenderer;
	};
}

