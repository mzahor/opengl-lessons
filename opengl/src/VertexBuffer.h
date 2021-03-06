#pragma once

#include "glutils.h"

class VertexBuffer
{
private:
    uint m_RendererId;

public:
    VertexBuffer(const void* data, uint size);
    ~VertexBuffer();
    void Bind() const;
    void Unbind() const;
};
