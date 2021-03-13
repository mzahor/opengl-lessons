#pragma once
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

class VertexArray
{
private:
    uint m_RendererId;
public:
    VertexArray();
    ~VertexArray();
    void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);
    const void Bind();
    const void Unbind();
};
