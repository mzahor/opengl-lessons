#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>

#define LOG(x) std::cout << x << std::endl


int loadFile(const char* filename, std::string& src) {
	std::ifstream file(filename, std::ios_base::in);
	if (!file) {
		std::cout << "Failed to open file " << filename << std::endl;
		return 1;
	}
	file.seekg(0, std::ios::end);
	src.reserve(file.tellg());
	file.seekg(0, std::ios::beg);
	src.assign((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
	file.close();
	return 0;
}

struct ShaderSource {
	std::string VertexSource;
	std::string FragmentSource;
};

ShaderSource ParseShader(const char* fname) {
	std::ifstream file(fname);
	std::string line;
	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1,
	};

	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(file, line))
	{
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}  else {
				type = ShaderType::FRAGMENT;
			}
			continue;
		} else if (type == ShaderType::NONE) {
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

unsigned int compileShader(const std::string& src, GLenum type) {
	unsigned int sid = glCreateShader(type);
	const char* source = src.c_str();
	glShaderSource(sid, 1, &source, nullptr);
	glCompileShader(sid);

	int success = GL_FALSE;
	glGetShaderiv(sid, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
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

unsigned int createProgram(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int pid = glCreateProgram();
	unsigned int v_sid = compileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int f_sid = compileShader(fragmentShader, GL_FRAGMENT_SHADER);
	glAttachShader(pid, v_sid);
	glAttachShader(pid, f_sid);
	glLinkProgram(pid);

	int success = GL_FALSE;
	glGetProgramiv(pid, GL_LINK_STATUS, &success);

	if (!success) {
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

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		LOG("Failed to create glfw window");
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		LOG("Failed to init glew");
	}
	else {
		LOG(glGetString(GL_VERSION));
	}

	float buf[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f,
	};

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), buf, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	int err = 0;
	std::string vertexShader;
	err = loadFile(".\\VertexShader.glsl", vertexShader);
	if (err) return err;
	std::string fragmentShader;
	err = loadFile(".\\FragmentShader.glsl", fragmentShader);
	if (err) return err;
	auto prog_id = createProgram(vertexShader, fragmentShader);

	/*auto shaders = ParseShader("./Basic.shader");
	auto prog_id = createProgram(shaders.VertexSource, shaders.FragmentSource);*/
	
	glUseProgram(prog_id);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
