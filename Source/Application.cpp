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
	    // Front face
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // Bottom-left
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // Bottom-right
	     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // Top-right
	    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // Top-left
	
	    // Back face
	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // Bottom-left
	    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  // Top-left
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // Top-right
	     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  // Bottom-right
	
	     // Left face
	     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // Bottom-left
	     -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // Bottom-right
	     -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // Top-right
	     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // Top-left
	
	     // Right face
	      0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // Bottom-left
	      0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  // Top-left
	      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // Top-right
	      0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  // Bottom-right
	
	      // Top face
	      -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  // Bottom-left
	       0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  // Bottom-right
	       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // Top-right
	      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // Top-left
	
	      // Bottom face
	      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // Bottom-left
	      -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // Bottom-right
	       0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  // Top-right
	       0.5f, -0.5f, -0.5f,  0.0f, 1.0f   // Top-left
};

static const unsigned int Indices[] =
{
	    0, 1, 2,  2, 3, 0,  // Front face
	    4, 5, 6,  6, 7, 4,  // Back face
	    8, 9, 10, 10, 11, 8, // Left face
	    12, 13, 14, 14, 15, 12, // Right face
	    16, 17, 18, 18, 19, 16, // Top face
	    20, 21, 22, 22, 23, 20  // Bottom face
};

const char* vertex_shader_text =
"#version 330\n"
"in vec3 vPos;\n"
"in vec2 TexPos;\n"
"out vec2 TexCoord;\n"
"uniform mat4 m_MVP;\n"
"void main()\n"
"{\n"
"    gl_Position = m_MVP * vec4(vPos, 1);\n"
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

	GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	// Texture Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

	// Depth Testing
	glEnable(GL_DEPTH_TEST);

	// Vertex Buffer
	VertexBuffer VB(vertices, sizeof(vertices));

	// Index Buffer
	IndexBuffer IB(Indices, sizeof(Indices));

	// Shaders
	Shader m_Shader(vertex_shader_text, fragment_shader_text);
	m_Shader.Bind();
	m_Shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.4f, 0.5f);

	// MVP
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);
	m_Shader.SetUniformMat4f("m_MVP", Projection);

	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	m_Shader.SetUniformMat4f("m_MVP", Projection * View);

	glm::mat4 Model = glm::mat4(1.0f);

	// Vertex Array
	VertexArray VA;

	// Texture
	Texture m_Texture("D:/OpenGL/Source/Textures/SM_Pillar_NN_01a_lambert2_Normal.png");
	m_Texture.Bind(0);

	m_Shader.SetUniform1i("u_Texture", 0);

	// Vertex Buffer Layout
	VertexBufferLayout VBL;
	VBL.PushData<float>(3);
	VBL.PushData<float>(2);
	VA.AddBuffer(VB, VBL);

	while (!glfwWindowShouldClose(window))
	{
		// Rotate
		Model = glm::rotate(Model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_Shader.SetUniformMat4f("m_MVP", Projection * View * Model);

		// Bind
		VA.Bind();
		IB.Bind();

		// Renderer
		Renderer m_Renderer(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// UnBind
	VB.UnBind();
	IB.UnBind();
	VA.UnBind();
	m_Shader.UnBind();
	m_Texture.UnBind();

	// Destroy, Terminate and Exit
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}