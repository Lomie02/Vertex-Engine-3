#pragma once
#include "Renderer.h"
#include "Scene.h"
namespace VertexEngine {

	class RenderSystem
	{
	public:
		RenderSystem(Renderer* _renderAPI);
		void SetActiveScene(Scene* _activeScene);

		void OnUpdate();
	private:
		void InitProps();
		Renderer* m_MainlineRenderer;
		Scene* m_ActiveScene;
	};
}

