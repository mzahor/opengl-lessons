#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include "glutils.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define LOG(x) std::cout << x << std::endl

static void glfwError(int id, const char *description)
{
	std::cout << "GLFW init error: " << description << std::endl;
}

int main(void)
{
	GLFWwindow *window;

	glfwSetErrorCallback(&glfwError);
	if (!glfwInit())
	{
		LOG("Failed to init glfw");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		LOG("Failed to create glfw window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		LOG("Failed to init glew");
	}
	else
	{
		LOG(glGetString(GL_VERSION));
	}

	{
		float vBuf[] = {
			-0.5f, -0.5f,
			0.5f, -0.5f,
			0.5f, 0.5f,
			-0.5f, 0.5f};

		uint iBuf[] = {
			0, 1,
			2, 2,
			3, 0};


		VertexArray vao;
		VertexBuffer vbo(vBuf, 4 * 2 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		vao.AddBuffer(vbo, layout);
		IndexBuffer ibo(iBuf, 6);
		Shader shader("./Basic.shader");
		Renderer renderer;

		float color[] = {0.0f, 0.3f, 0.7f, 1.0f};
		float inc[] = {0.05f, 0.03f, 0.07f};

		glfwSwapInterval(1);

		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();

			for (int i = 0; i < 3; i++)
			{
				if (color[i] > 1.0f || color[i] < 0.0)
				{
					inc[i] = -inc[i];
				}
				color[i] += inc[i];
			}

			shader.Bind();
			shader.SetUniform4f("u_color", color[0], color[1], color[2], color[3]);

			renderer.Draw(vao, ibo, shader);

			GLCall(glfwSwapBuffers(window));
			GLCall(glfwPollEvents());
		}
	}
	glfwTerminate();
	return 0;
}
