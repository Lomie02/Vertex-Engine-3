#include "pch.h"
#include "BackendFactory.h"
#include "GlWindow.h"
#include "GlfwInputSystem.h"
std::unique_ptr<VertexEngine::Window> BackendFactory::CreateWindow(VertexEngine::GraphicsAPI _api, int width, int height)
{
	switch (_api) {
	case VertexEngine::GraphicsAPI::OpenGL:
		return std::make_unique<VertexEngine::GlWindow>(width, height);
		break;
	case VertexEngine::GraphicsAPI::Vulkan:
		break;
	case VertexEngine::GraphicsAPI::DirectX:
		break;
	}

	// Failed to create window
	return nullptr;
}

std::unique_ptr<VertexEngine::InputSystem> BackendFactory::CreateInput(VertexEngine::GraphicsAPI _api, VertexEngine::Window* _window)
{
	switch (_api) {
	case VertexEngine::GraphicsAPI::OpenGL:
		return std::make_unique<VertexEngine::GlfwInputSystem>(static_cast<VertexEngine::GlWindow*>(_window));
		break;
	case VertexEngine::GraphicsAPI::Vulkan:
		break;
	case VertexEngine::GraphicsAPI::DirectX:
		break;
	}

	return nullptr;

}
