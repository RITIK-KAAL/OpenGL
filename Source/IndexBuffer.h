#pragma once

class IndexBuffer
{
private:

	unsigned int m_RendererID;
	unsigned int m_Count;

public:

	IndexBuffer(const unsigned int* Data, unsigned int count);

	~IndexBuffer();

	const void Bind();

	const void UnBind();

	inline const unsigned int GetCount() const { return m_Count; }

};
