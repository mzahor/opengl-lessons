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
			-50.0f, -50.0f, 0.0f, 0.0f,
			50.0f, -50.0f, 1.0f, 0.0f,
			50.0f, 50.0f, 1.0f, 1.0f,
			-50.0f, 50.0f, 0.0f, 1.0f};

		uint iBuf[] = {
			0, 1,
			2, 2,
			3, 0};

		glm::mat4 proj = glm::ortho(-200.0f, 200.0f, -150.0f, 150.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 50.0f, 0.0f));
		glm::mat4 mvp = proj * view * model;

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray vao;
		VertexBuffer vbo(vBuf, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		vao.AddBuffer(vbo, layout);
		IndexBuffer ibo(iBuf, 6);
		Shader shader("./Basic.shader");
		Texture texture("./cherno.png");
		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		const char* glsl_version = "#version 130";
		ImGui_ImplOpenGL3_Init(glsl_version);
		ImGui::StyleColorsDark();
		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		texture.Bind(0);
		shader.Bind();
		shader.SetUniform1i("u_Texture", 0);
		shader.SetUniformMat4f("u_MVP", mvp);

		glfwSwapInterval(1);

		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text.");		   // Display some text (you can use a format strings too)
				ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
				ImGui::Checkbox("Another Window", &show_another_window);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);			 // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

				if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			renderer.Draw(vao, ibo, shader);

			GLCall(glfwSwapBuffers(window));
			GLCall(glfwPollEvents());
		}
	}

	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}
