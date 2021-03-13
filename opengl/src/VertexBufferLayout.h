#pragma once
#include <vector>
#include "Renderer.h"

struct VertexBufferElement
{
    int count;
    uint type;
    uint normalized;

    static uint GetSizeOfType(uint type)
    {
        switch (type)
        {
        case GL_FLOAT:
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        }
        ASSERT(false);
        return 0;
    }

    static inline int CalcElemSize(int count, uint type)
    {
        return count * GetSizeOfType(type);
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    int m_Stride;

    void PushElement(int count, uint type, unsigned char normalized)
    {
        m_Elements.push_back({count, type, normalized});
        m_Stride += VertexBufferElement::CalcElemSize(count, type);
    }

public:
    template <typename T>
    void Push(int count)
    {
        static_assert(false);
    }

    template <>
    void Push<float>(int count)
    {
        PushElement(count, GL_FLOAT, false);
    }

    template <>
    void Push<uint>(int count)
    {
        PushElement(count, GL_UNSIGNED_INT, false);
    }

    template <>
    void Push<unsigned char>(int count)
    {
        PushElement(count, GL_UNSIGNED_BYTE, false);
    }

    inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
    inline const int GetStride() const { return m_Stride; }
};
