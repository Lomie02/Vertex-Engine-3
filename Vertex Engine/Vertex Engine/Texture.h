#pragma once

#include <vector>
#include "Asset.h"

namespace VertexEngine {

	class Texture : public VertexEngine::Asset
	{
	public:

		void Load(const std::string& _path) override; // load the texure into memory.

		const std::vector<unsigned char>& GetPixels() const { return m_Pixels; }// get the pixels of the image
		int GetWidth() const { return m_Width; } // Get the texures width
		int GetHeight() const { return m_height; } // Get the texures height
		int GetChannelds() const { return m_Channels; } // get the channels the texure has

	private:
		std::vector<unsigned char> m_Pixels; // Texture pixels
		int m_Width = 0; // Textures Wdith
		int m_height = 0; // Texures Height
		int m_Channels = 0; // Channels that the texture has
	};
}

