#pragma once

#include <D:/OpenGL/OpenGl/Dependencies/include/GLFW/glfw3.h>
#include<vector>

struct BufferHolder
{
	unsigned int Type;
	unsigned int Size;
	char Normalized;

	static unsigned int GetType(unsigned int Type)
	{ 
		switch (Type)
		{
		case GL_INT :
			return 4;
		case GL_FLOAT:
			return 4;
		case GL_BYTE:
			return 1;
		}
		return 0;
	}
};

class VertexBufferLayout
{
private:

	std::vector<BufferHolder> BufferElements;
	unsigned int m_Stride;

public:

	VertexBufferLayout() : m_Stride(0) {};

	template<typename T> 
	void PushData(unsigned int Count)
	{
		std::_Xruntime_error(false);
	}

	template<>
	void PushData<unsigned int>(unsigned int Count)
	{
		BufferElements.push_back({ GL_INT, Count, GL_FALSE });
		m_Stride += Count * BufferHolder::GetType(GL_INT);
	}

	template<>
	void PushData<float>(unsigned int Count)
	{
		BufferElements.push_back({ GL_FLOAT, Count, GL_FALSE });
		m_Stride += Count * BufferHolder::GetType(GL_FLOAT);
	}

	template<>
	void PushData<char>(unsigned int Count)
	{
		BufferElements.push_back({ GL_BYTE, Count, GL_TRUE });
		m_Stride += Count * BufferHolder::GetType(GL_BYTE);
	}

	const inline std::vector<BufferHolder>GetElements() const { return BufferElements; }

	const inline unsigned int GetStride() const { return m_Stride; }
};
