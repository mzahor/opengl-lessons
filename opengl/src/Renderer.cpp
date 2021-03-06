#include <iostream>
#include "Renderer.h"

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray &vao, IndexBuffer &ibo, const Shader &shader) const
{
	vao.Bind();
	ibo.Bind();
	shader.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}
