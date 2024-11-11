#include <Source/deps/glad/gl.h>
#include <Dependencies/include/GLFW/glfw3.h>
#include "Shader.h"

Shader::Shader(const char* vertexSource, const char* fragmentSource) 
{
    m_VertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    m_FragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    m_RendererId = glCreateProgram();
    glAttachShader(m_RendererId, m_VertexShader);
    glAttachShader(m_RendererId, m_FragmentShader);
    glLinkProgram(m_RendererId);

    glDeleteShader(m_VertexShader);
    glDeleteShader(m_FragmentShader);
}

Shader::~Shader() 
{
    glDeleteProgram(m_RendererId);
}

void Shader::Bind() const 
{
    glUseProgram(m_RendererId);
}

void Shader::UnBind() const
{
    glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) 
{
    GLint location = glGetUniformLocation(m_RendererId, name.c_str());
    glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& Matrix)
{
    GLint location = glGetUniformLocation(m_RendererId, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &Matrix[0][0]);
}

unsigned int Shader::CompileShader(GLenum type, const char* source) 
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    return shader;
}

void Shader::SetUniform1i(const std::string& name, unsigned int v0)
{
    GLint location = glGetUniformLocation(m_RendererId, name.c_str());
    glUniform1f(location, v0);
}
