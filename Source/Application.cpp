#define GLAD_GL_IMPLEMENTATION
#include <D:/OpenGL/OpenGl/Source/deps/glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <D:/OpenGL/OpenGl/Dependencies/include/GLFW/glfw3.h>

#include "D:/OpenGL/OpenGl/Source/deps/linmath.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

static const float vertices[] =
{
	-0.5f, -0.5f, 
	0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, 0.5f,
};

static const unsigned int Indices[] =
{
	0, 1, 2, 
	3, 0, 2
};

const char* vertex_shader_text =
"#version 330\n"
"in vec2 vPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(vPos, 0.0, 1.0);\n"
"}\n";

const char* fragment_shader_text =
"#version 330\n"
"out vec4 fragment;\n"
"uniform vec4 u_Color;"
"void main()\n"
"{\n"
"    fragment = u_Color;\n"
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

	VertexBuffer VB(vertices,sizeof(float) * 6 * 2);

	IndexBuffer IB(Indices, 6);

	Shader m_Shader(vertex_shader_text, fragment_shader_text);

	m_Shader.Bind();

	m_Shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.4f, 0.5f);

	VertexArray VA;

	VertexBufferLayout VBL;

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