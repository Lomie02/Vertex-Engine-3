#include "pch.h"
#include "BackendFactory.h"
#include "GlWindow.h"
#include "GlfwInputSystem.h"
#include "GLRenderer.h"
std::unique_ptr<VertexEngine::Window> VertexEngine::BackendFactory::CreateWindow(VertexEngine::GraphicsAPI _api, int width, int height)
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

	throw std::runtime_error("Window API Failed!");
}

std::unique_ptr<VertexEngine::InputSystem> VertexEngine::BackendFactory::CreateInput(VertexEngine::GraphicsAPI _api, VertexEngine::Window* _window)
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

	throw std::runtime_error("Input API Failed!");

}

std::unique_ptr<VertexEngine::Renderer> VertexEngine::BackendFactory::CreateRenderer(VertexEngine::GraphicsAPI _api, VertexEngine::Window* _window, VertexEngine::AssetManager* _assetManager)
{
	switch (_api) {
	case VertexEngine::GraphicsAPI::OpenGL:
		return std::make_unique<VertexEngine::GLRenderer>(static_cast<VertexEngine::GlWindow*>(_window), _assetManager->Get<Shader>("gl_default_shader_vertex"), _assetManager->Get<Shader>("gl_default_shader_fragment"));
		break;
	case VertexEngine::GraphicsAPI::Vulkan:
		break;
	case VertexEngine::GraphicsAPI::DirectX:
		break;
	}

	throw std::runtime_error("Renderer API Failed!");
}
