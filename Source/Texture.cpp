#include "Texture.h"
#include <D:/OpenGL/Source/External/Stb_Image.h>
#include <D:/OpenGL/Source/deps/glad/gl.h>

Texture::Texture(const char& TextureLocation) : m_Renderer(0), m_TextureLocation(TextureLocation), m_BufferHolder(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);

	m_BufferHolder = stbi_load(&TextureLocation, &m_Width, &m_Height, &m_BPP, 4);

	glGenTextures(1, &m_Renderer);
	glBindTexture(GL_TEXTURE_2D, m_Renderer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_BufferHolder);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_BufferHolder)
	{
		stbi_image_free(m_BufferHolder);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_Renderer);
}

void Texture::Bind(const unsigned int Coord) const
{
	glActiveTexture(GL_TEXTURE0 + Coord);
	glBindTexture(GL_TEXTURE_2D, m_Renderer);
}

void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
