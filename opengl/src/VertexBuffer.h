#pragma once

#include "Renderer.h"

class VertexBuffer
{
private:
    uint m_RendererId;

public:
    VertexBuffer(const void* data, uint size);
    ~VertexBuffer();
    void Bind();
    void Unbind();
};
