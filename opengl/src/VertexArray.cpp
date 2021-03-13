#include "VertexArray.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererId));
    Bind();
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererId));
}

void VertexArray::AddBuffer(const VertexBuffer &vbo, const VertexBufferLayout &layout)
{
    vbo.Bind();
    const auto &elements = layout.GetElements();

    uint offset = 0;
    for (uint i = 0; i < elements.size(); i++)
    {
        const auto &element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (GLvoid *)offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

const void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererId));
}

const void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
