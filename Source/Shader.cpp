#include <D:/OpenGL/Source/deps/glad/gl.h>
#include <D:/OpenGL/Dependencies/include/GLFW/glfw3.h>
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

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) 
{
    GLint location = glGetUniformLocation(m_RendererId, name.c_str());
    glUniform4f(location, v0, v1, v2, v3);
}

unsigned int Shader::CompileShader(GLenum type, const char* source) 
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    return shader;
}