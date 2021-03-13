#pragma once

#include "glutils.h"

class IndexBuffer
{
private:
	uint m_RendererId;
	uint m_Count;

public:
	IndexBuffer(const uint* data, uint count);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;
	uint GetCount() const;
};
