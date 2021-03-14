#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "glm/glm.hpp"

Shader::Shader(const std::string &filepath) : m_ShaderPath(filepath), m_RendererId(0)
{
    auto shaderSource = ParseShader(filepath);
    m_RendererId = CreateProgram(shaderSource.VertexSource, shaderSource.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererId));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererId));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string &name, float v1, float v2, float v3, float v4)
{
    int location = GetUniformLocation(name);
    GLCall(glUniform4f(location, v1, v2, v3, v4));
}

void Shader::SetUniform1i(const std::string &name, int value)
{
    int location = GetUniformLocation(name);
    GLCall(glUniform1i(location, value));
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 matrix)
{
    int location = GetUniformLocation(name);
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string &name)
{
    int location;

    if (m_LocationCache.find(name) == m_LocationCache.end())
    {
        location = glGetUniformLocation(m_RendererId, name.c_str());
    }
    else
    {
        location = m_LocationCache[name];
    }

    return location;
}

uint Shader::CreateProgram(const std::string &vertexShader, const std::string &fragmentShader)
{
    uint pid = glCreateProgram();
    uint v_sid = CompileShader(vertexShader, GL_VERTEX_SHADER);
    uint f_sid = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);
    GLCall(glAttachShader(pid, v_sid));
    GLCall(glAttachShader(pid, f_sid));
    GLCall(glLinkProgram(pid));

    int success = GL_FALSE;
    GLCall(glGetProgramiv(pid, GL_LINK_STATUS, &success));

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

uint Shader::CompileShader(const std::string &src, GLenum type)
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

ShaderSource Shader::ParseShader(const std::string &fname)
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
