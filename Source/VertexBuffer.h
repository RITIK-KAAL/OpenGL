#pragma once

class VertexBuffer
{
private :

	unsigned int m_RendererID;

public : 

	VertexBuffer(const void* Data, unsigned int Size);

	~VertexBuffer();

	const void Bind();

	const void UnBind();

};
