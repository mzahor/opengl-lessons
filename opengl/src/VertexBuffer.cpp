#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, uint size)
{
    GLCall(glGenBuffers(1, &m_RendererId));
    VertexBuffer::Bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererId));
}

void VertexBuffer::Bind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
}

void VertexBuffer::Unbind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
