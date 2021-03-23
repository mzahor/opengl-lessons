#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const uint *data, uint count) : m_Count(count)
{
	GLCall(glGenBuffers(1, &m_RendererId));
	IndexBuffer::Bind();
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * count, data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererId));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

inline uint IndexBuffer::GetCount() const { return m_Count; }
