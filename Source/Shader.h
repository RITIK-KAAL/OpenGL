#include <D:/OpenGL/Dependencies/include/GLFW/glfw3.h>
#include <D:/OpenGL/Source/External/glm/glm.hpp>
#include <string>
#include <iostream>

class Shader {

private:
    unsigned int  m_VertexShader;
    unsigned int m_FragmentShader;
    unsigned int m_RendererId;

public:
    Shader(const char* vertexSource, const char* fragmentSource);

    ~Shader();

    void Bind() const;
    void Unbind() const;

    unsigned int CompileShader(GLenum type, const char* source);

    void SetUniform1i(const std::string& name, unsigned int v0);

    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

    void SetUniformMat4f(const std::string& name, const glm::mat4& Matrix);
};