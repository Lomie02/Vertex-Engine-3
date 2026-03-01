#pragma once
#include <memory>
#include "Window.h"
#include "GraphicsAPI.h"
#include "InputSystem.h"
#include "Renderer.h"
#include "AssetManager.h"

namespace VertexEngine {

	class BackendFactory
	{
	public:
		/// <summary>
		/// Create & return the window based on the given API.
		/// </summary>
		/// <param name="_api"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <returns></returns>
		std::unique_ptr<VertexEngine::Window> CreateWindow(VertexEngine::GraphicsAPI _api, int width, int height);
		/// <summary>
		///  Create & return the inputsystem based on the given API.
		/// </summary>
		/// <param name="_api"></param>
		/// <param name="_window"></param>
		/// <returns></returns>
		std::unique_ptr<VertexEngine::InputSystem> CreateInput(VertexEngine::GraphicsAPI _api, VertexEngine::Window* _window);
		std::unique_ptr<VertexEngine::Renderer> CreateRenderer(VertexEngine::GraphicsAPI _api, VertexEngine::Window* _window, VertexEngine::AssetManager* _assetManager);
	};
}

