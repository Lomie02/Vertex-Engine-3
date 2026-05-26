#pragma once
#include "Renderer.h"
#include "Scene.h"
namespace VertexEngine {

	class RenderSystem
	{
	public:
		RenderSystem(Renderer* _renderAPI);

		void OnSceneChanged(Scene* _scene);
		void OnUpdate();
	private:
		void InitProps();

		Renderer* m_MainlineRenderer;
		Scene* m_ActiveScene;
	};
}

