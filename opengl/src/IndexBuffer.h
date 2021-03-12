#pragma once

#include "Renderer.h"

class IndexBuffer
{
private:
    uint m_RendererId;

public:
    IndexBuffer(const void* data, uint size);
    ~IndexBuffer();
    void Bind();
    void Unbind();
};
