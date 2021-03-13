#pragma once
#include "glutils.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray &vao, IndexBuffer &ibo, const Shader &shader) const;
};
