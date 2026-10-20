#pragma once
#include <memory>
#include "Renderable.h"
#include "Texture.h"
#include "CameraRenderable.h"
namespace VertexEngine {

	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual void ClearFrame() = 0; // Clear render flags.
		virtual void BeginFrame() = 0; // Begin the current frame for rendering
		virtual void Submit(const Renderable& _mesh) = 0; // Submit & unpacks rendering data
		virtual void Render() = 0; // Submit & unpacks rendering data
		virtual void EndFrame() = 0; // End of frame, update polls

		virtual void SubmitCamera(const CameraRenderable& _camera) = 0; // Submit the active camera to the renderer

		virtual void BindTexture(std::shared_ptr<VertexEngine::Texture> _texture) = 0; // Bind texture to the gpu


		// Debugging data
		void SetPassChange() { m_SetPasses++; } // Increase set passes
		int GetSetPasses() { return m_SetPasses; } // Get the current amount of set passes

		void BatchesChange() { m_Batches++; } // Increase Batches
		int GetBatches() { return m_Batches; } // Get the current amount of batches


		void ClearStatisticData() { // Clear all debugging data
			m_SetPasses = 0;
			m_Batches = 0;
		}

	private:
		int m_SetPasses = 0;
		int m_Batches = 0;
	};
}

