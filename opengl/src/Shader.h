#pragma once
#include <string>
#include <unordered_map>
#include "glutils.h"

struct ShaderSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    uint m_RendererId;
    std::string m_ShaderPath;
    std::unordered_map<std::string, uint> m_LocationCache;

public:
    Shader(const std::string &filepath);
    ~Shader();

    void SetUniform1i(const std::string& name, int value);
    void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4);

    void Bind() const;
    void Unbind() const;

private:
    uint CreateProgram(const std::string &vertexShader, const std::string &fragmentShader);
    uint CompileShader(const std::string &src, GLenum type);
    ShaderSource ParseShader(const std::string &fname);
    int GetUniformLocation(const std::string &name);
};
