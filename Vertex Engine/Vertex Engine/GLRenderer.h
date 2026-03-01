#pragma once
#include "GLWindow.h"
#include "Renderer.h"
#include <unordered_map>
#include "Shader.h"
#include <map>
namespace VertexEngine {
	class GLRenderer : public VertexEngine::Renderer
	{
	public:
		GLRenderer(GlWindow* _win, std::shared_ptr<Shader> _defaultVertex, std::shared_ptr<Shader> _defaultFrag); // Takes in the window alongside the default opengl shaders. Provided by backend.

		void BeginFrame() override; // Prepare start of frame.
		void ClearFrame() override; // Clear flags
		void Submit(const Renderable& _mesh) override; // Submit renderables to the render queue
		void Render() override; // Render everything in queue
		void EndFrame() override; // End frame & switch polls

		void BindTexture(std::shared_ptr<Texture> _texture) override; // Bind textures

		// Shader
		unsigned int CompileProgram(std::shared_ptr<Shader> _vertex, std::shared_ptr<Shader> _frag); // Compile the shader program
		void UseShader(std::shared_ptr<Shader> _vertex, std::shared_ptr<Shader> _frag); // Use shader
	private:
		unsigned int CompileShader(unsigned int type, const std::string& source); // Compile the shaders
		unsigned int UploadTexture(std::shared_ptr<Texture> _texture); // Upload texture to gpu

		GlWindow* m_WindowHandle; // Game window

		std::vector<Renderable> m_RenderQueue; // Render queue for renderering
		std::unordered_map<std::shared_ptr<Texture>, unsigned int >m_gpuHandle; //lazy loading for textures
		std::map<std::pair<std::shared_ptr<Shader>, std::shared_ptr<Shader>>, unsigned int > m_Programs; // Lazy loading for shaders

		// Default OpenGL shaders
		std::shared_ptr<Shader> m_DefaultVertexShader; 
		std::shared_ptr<Shader> m_DefaultFragShader;

		// GLRenderer.h
		unsigned int m_TriangleVAO = 0;
		unsigned int m_TriangleVBO = 0;
	};
}

