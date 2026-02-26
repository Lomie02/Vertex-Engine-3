#pragma once
#include <memory>
#include "Window.h"
#include "GraphicsAPI.h"
#include "InputSystem.h"
class BackendFactory
{
public:
	std::unique_ptr<VertexEngine::Window> CreateWindow(VertexEngine::GraphicsAPI _api, int width, int height);

	std::unique_ptr<VertexEngine::InputSystem> CreateInput(VertexEngine::GraphicsAPI _api, VertexEngine::Window* _window);



};

