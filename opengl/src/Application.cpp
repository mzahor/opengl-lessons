#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"

#define LOG(x) std::cout << x << std::endl

static int loadFile(const char *filename, std::string &src)
{
	std::ifstream file(filename, std::ios_base::in);
	if (!file)
	{
		std::cout << "Failed to open file " << filename << std::endl;
		return 1;
	}
	file.seekg(0, std::ios::end);
	src.reserve((uint)file.tellg());
	file.seekg(0, std::ios::beg);
	src.assign((std::istreambuf_iterator<char>(file)),
			   std::istreambuf_iterator<char>());
	file.close();
	return 0;
}

struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderSource ParseShader(const char *fname)
{
	std::ifstream file(fname);
	std::string line;
	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1,
	};

	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(file, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else
			{
				type = ShaderType::FRAGMENT;
			}
			continue;
		}
		else if (type == ShaderType::NONE)
		{
			continue;
		}
		ss[(int)type] << line << "\n";
	}
	ShaderSource source = {
		ss[(int)ShaderType::VERTEX].str(),
		ss[(int)ShaderType::FRAGMENT].str(),
	};
	return source;
}

static uint compileShader(const std::string &src, GLenum type)
{
	uint sid = glCreateShader(type);
	const char *source = src.c_str();
	glShaderSource(sid, 1, &source, nullptr);
	glCompileShader(sid);

	int success = GL_FALSE;
	glGetShaderiv(sid, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		int logSize = 0;
		glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &logSize);
		char *errMsg = (char *)alloca(logSize);
		glGetShaderInfoLog(sid, logSize, &logSize, errMsg);
		glDeleteShader(sid);
		std::cout << errMsg << std::endl;
		return 0;
	}

	return sid;
}

static uint createProgram(const std::string &vertexShader, const std::string &fragmentShader)
{
	uint pid = glCreateProgram();
	uint v_sid = compileShader(vertexShader, GL_VERTEX_SHADER);
	uint f_sid = compileShader(fragmentShader, GL_FRAGMENT_SHADER);
	glAttachShader(pid, v_sid);
	glAttachShader(pid, f_sid);
	glLinkProgram(pid);

	int success = GL_FALSE;
	glGetProgramiv(pid, GL_LINK_STATUS, &success);

	if (!success)
	{
		int logSize = 0;
		glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &logSize);
		char *errMsg = (char *)alloca(logSize);
		glGetProgramInfoLog(pid, logSize, &logSize, errMsg);
		glDeleteProgram(pid);
		std::cout << errMsg << std::endl;
		return 0;
	}

	glDetachShader(pid, v_sid);
	glDetachShader(pid, f_sid);

	return pid;
}

uint createProgramImpl()
{
	int err = 0;
	std::string vertexShader;
	err = loadFile("./VertexShader.glsl", vertexShader);
	if (err)
		return err;
	std::string fragmentShader;
	err = loadFile("./FragmentShader.glsl", fragmentShader);
	if (err)
		return err;
	auto id = createProgram(vertexShader, fragmentShader);

	// auto shaders = ParseShader("./Basic.shader");
	// auto id = createProgram(shaders.VertexSource, shaders.FragmentSource);
	return id;
}

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

		uint vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		VertexBuffer vbo(vBuf, 4 * 2 * sizeof(float));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

		uint ibo;
		GLCall(glGenBuffers(1, &ibo));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint), iBuf, GL_STATIC_DRAW));

		uint prog_id = createProgramImpl();
		int u_color = glGetUniformLocation(prog_id, "u_color");
		GLCall(glUseProgram(prog_id));

		float color[] = {0.0f, 0.3f, 0.7f, 1.0f};
		float inc[] = {0.05f, 0.03f, 0.07f};

		glfwSwapInterval(1);

		GLCall(glUseProgram(0));
		GLCall(glBindVertexArray(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		while (!glfwWindowShouldClose(window))
		{
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			for (int i = 0; i < 3; i++)
			{
				if (color[i] > 1.0f || color[i] < 0.0)
				{
					inc[i] = -inc[i];
				}
				color[i] += inc[i];
			}

			GLCall(glUseProgram(prog_id));
			GLCall(glUniform4fv(u_color, 1, color));

			GLCall(glBindVertexArray(vao));

			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
			GLCall(glfwSwapBuffers(window));
			GLCall(glfwPollEvents());
		}
	}
	glfwTerminate();
	return 0;
}
