#pragma once

#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

class VertexArray
{
private: 

	unsigned int m_Renderer;

public:

	VertexArray();

	~VertexArray();

	void AddBuffer(VertexBuffer& VA, VertexBufferLayout& VBL);

	const void Bind();

	const void UnBind();
};
