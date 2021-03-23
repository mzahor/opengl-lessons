#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <string>
#include <sstream>
#include "glutils.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "tests/ColorFillTest.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#define LOG(x) std::cout << x << std::endl

static void glfwError(int id, const char *description)
{
	std::cout << "GLFW init error: " << description << std::endl;
}

void initImGui(GLFWwindow *window)
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char *glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();
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

	initImGui(window);

	test::ColorFillTest test;
	glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window))
	{
		test.OnUpdate(0.0f);
		test.OnRender();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		test.OnImGuiRender();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		GLCall(glfwSwapBuffers(window));
		GLCall(glfwPollEvents());
	}

	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}
