#pragma once
#include <memory>
#include "iRenderer.h"

namespace VertexEngine {

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void RenderFrame();

	private:
		std::unique_ptr<iRenderer> m_Renderer;
	};
}

