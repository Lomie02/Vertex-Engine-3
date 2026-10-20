#pragma once
#include "Renderer.h"
#include "Scene.h"
namespace VertexEngine {

	class RenderSystem
	{
	public:
		RenderSystem(Renderer* _renderAPI);

		void OnSceneChanged(Scene* _scene);
		void OnWindowChanged(int _aspect);
		void OnUpdate();

	private:
		void InitProps();

		Renderer* m_MainlineRenderer;
		Scene* m_ActiveScene;

		float m_AspectRatio = 1.7f; // Set default aspect to a ratio of 1920 x 1080
	};
}

