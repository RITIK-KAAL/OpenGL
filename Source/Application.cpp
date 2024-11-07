#define GLAD_GL_IMPLEMENTATION
#include <D:/OpenGL/Source/deps/glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <D:/OpenGL/Dependencies/include/GLFW/glfw3.h>
#include <D:/OpenGL/Source/External/glm/glm.hpp>
#include <D:/OpenGL/Source/External/glm/gtc/matrix_transform.hpp>

#include <D:/OpenGL/Source/deps/linmath.h>
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

static const float vertices[] =
{
	-0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 1.0f, 0.0f, 
	0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.0f, 1.0f
};

static const unsigned int Indices[] =
{
	0, 1, 2, 
	3, 0, 2
};

const char* vertex_shader_text =
"#version 330\n"
"in vec4 vPos;\n"
"in vec2 TexPos;\n"
"out vec2 TexCoord;\n"
"uniform mat4 m_MVP;\n"
"void main()\n"
"{\n"
"    gl_Position = vPos * m_MVP;\n"
"    TexCoord = TexPos;\n"
"}\n";

const char* fragment_shader_text =
"#version 330\n"
"out vec4 fragment;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D u_Texture;\n"
"uniform vec4 u_Color;\n"
"void main()\n"
"{\n"
"    vec4 Color = texture(u_Texture, TexCoord);\n"
"    fragment = Color;\n"
"}\n";

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

	VertexBuffer VB(vertices,sizeof(float) * 4 * 4);

	IndexBuffer IB(Indices, 6);

	Shader m_Shader(vertex_shader_text, fragment_shader_text);

	m_Shader.Bind();

	m_Shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.4f, 0.5f);

	glm::mat4 MVP = glm::ortho(-1.5f, 1.5f, -1.5f, 1.5f, -1.5f, 1.5f);

	m_Shader.SetUniformMat4f("m_MVP", MVP);

	VertexArray VA;

	Texture m_Texture("D:/OpenGL/Source/Textures/SM_Pillar_NN_01a_lambert2_Normal.png");

	m_Texture.Bind(0);

	m_Shader.SetUniform1i("u_Texture", 0);

	VertexBufferLayout VBL;

	VBL.PushData<float>(2);
	VBL.PushData<float>(2);
	VA.AddBuffer(VB, VBL);

	while (!glfwWindowShouldClose(window))
	{
		VA.Bind();
		IB.Bind();

		Renderer m_Renderer(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}