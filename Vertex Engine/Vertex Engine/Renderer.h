#pragma once
#include <memory>
#include "Renderable.h"
#include "Texture.h"

namespace VertexEngine {

	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual void ClearFrame() = 0;
		virtual void BeginFrame() = 0; // Begin the current frame for renderering
		virtual void Submit(const Renderable& _mesh) = 0; // Submit & unpacks rendering data
		virtual void Render() = 0; // Submit & unpacks rendering data
		virtual void EndFrame() = 0;

		virtual void BindTexture(std::shared_ptr<VertexEngine::Texture> _texture) = 0; // Bind texture to the gpu

	};
}

