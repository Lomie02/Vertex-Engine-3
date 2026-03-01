#include "pch.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void VertexEngine::Texture::Load(const std::string& _path)
{
	// Flip the image for OpenGL & Direct X.
	stbi_set_flip_vertically_on_load(true);

	if (m_Pixels.data()) stbi_image_free(m_Pixels.data());

	unsigned char* data = stbi_load(_path.c_str(), &m_Width, &m_height, &m_Channels, 0);

	// If asset failed to load then return.
	if (!data) {
		m_AssetLoaded = false;
		return;
	}

	size_t size = m_Width * m_height * m_Channels;
	m_Pixels.resize(size);
	std::memcpy(m_Pixels.data(), data, size);

	stbi_image_free(data);

	// Asset was loaded.
	m_AssetLoaded = true;
}
