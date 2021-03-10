#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>

#define LOG(x) std::cout << x << std::endl

#define ASSERT(X) if ((X)) __debugbreak()

#define GLCall(X)    \
	GLCleanErrors(); \
	X;               \
	ASSERT(GLPrintErrors(#X, __FILE__, __LINE__))

static void GLCleanErrors()
{
	while (glGetError() != GL_NO_ERROR)
	{
	}
}

static GLenum GLPrintErrors(const char* function, const char* file, long line)
{
	while (GLenum err = glGetError())
	{
		std::cout << "[OpenGL Error]: " << err << " " << function << " " << " at " << file << ":" << line << std::endl;
		return err;
	}
	return 0;
}

static int loadFile(const char* filename, std::string& src)
{
	std::ifstream file(filename, std::ios_base::in);
	if (!file)
	{
		std::cout << "Failed to open file " << filename << std::endl;
		return 1;
	}
	file.seekg(0, std::ios::end);
	src.reserve((unsigned int)file.tellg());
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

static ShaderSource ParseShader(const char* fname)
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

static unsigned int compileShader(const std::string& src, GLenum type)
{
	unsigned int sid = glCreateShader(type);
	const char* source = src.c_str();
	glShaderSource(sid, 1, &source, nullptr);
	glCompileShader(sid);

	int success = GL_FALSE;
	glGetShaderiv(sid, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		int logSize = 0;
		glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &logSize);
		char* errMsg = (char*)alloca(logSize);
		glGetShaderInfoLog(sid, logSize, &logSize, errMsg);
		glDeleteShader(sid);
		std::cout << errMsg << std::endl;
		return 0;
	}

	return sid;
}

static unsigned int createProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int pid = glCreateProgram();
	unsigned int v_sid = compileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int f_sid = compileShader(fragmentShader, GL_FRAGMENT_SHADER);
	glAttachShader(pid, v_sid);
	glAttachShader(pid, f_sid);
	glLinkProgram(pid);

	int success = GL_FALSE;
	glGetProgramiv(pid, GL_LINK_STATUS, &success);

	if (!success)
	{
		int logSize = 0;
		glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &logSize);
		char* errMsg = (char*)alloca(logSize);
		glGetProgramInfoLog(pid, logSize, &logSize, errMsg);
		glDeleteProgram(pid);
		std::cout << errMsg << std::endl;
		return 0;
	}

	glDetachShader(pid, v_sid);
	glDetachShader(pid, f_sid);

	return pid;
}

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		LOG("Failed to init glfw");
		return -1;
	}

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

	float vBuf[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f };

	unsigned int iBuf[] = {
		0, 1,
		2, 2,
		3, 0 };

	unsigned int vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vBuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), iBuf, GL_STATIC_DRAW);

	int err = 0;
	std::string vertexShader;
	err = loadFile("./VertexShader.glsl", vertexShader);
	if (err)
		return err;
	std::string fragmentShader;
	err = loadFile("./FragmentShader.glsl", fragmentShader);
	if (err)
		return err;
	auto prog_id = createProgram(vertexShader, fragmentShader);

	// auto shaders = ParseShader("./Basic.shader");
	// auto prog_id = createProgram(shaders.VertexSource, shaders.FragmentSource);

	glUseProgram(prog_id);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
