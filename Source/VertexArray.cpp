#include <D:/OpenGL/OpenGl/Source/deps/glad/gl.h>
#include "VertexBuffer.h"
#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_Renderer);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_Renderer);
}

void VertexArray::AddBuffer(VertexBuffer& VB, VertexBufferLayout& VBL)
{
	Bind();
	VB.Bind();
	unsigned int Offset = 0;
	const auto& Elements = VBL.GetElements();
	for (int i = 0; i < Elements.size(); i++)
	{
		const auto& Element = Elements[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, Element.Size, Element.Type, Element.Normalized, VBL.GetStride(), (const void*)Offset);

		Offset += Element.Size * Element.GetType(Element.Type);
	}

}

const void VertexArray::Bind()
{
	glBindVertexArray(m_Renderer);
}

const void VertexArray::UnBind()
{
	glBindVertexArray(0);
}
