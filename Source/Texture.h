#pragma once

class Texture
{
private : 

	unsigned int m_Renderer;
	const char m_TextureLocation;
	unsigned char* m_BufferHolder;
	int m_Width; 
	int m_Height; 
	int m_BPP;

public:

	Texture(const char& TextureLocation); 
	~Texture();

	void Bind(const unsigned int Coord) const;
	void UnBind() const;

	inline const unsigned int GetWidth() const { return m_Width; }

	inline const unsigned int GetHeight() const { return m_Height; }
};