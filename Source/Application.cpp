#define GLAD_GL_IMPLEMENTATION
#include <D:/OpenGL/Source/deps/glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <Dependencies/include/GLFW/glfw3.h>
#include <Source/External/glm/glm.hpp>
#include <Source/External/glm/gtc/matrix_transform.hpp>
#include <Source/deps/linmath.h>
#include <Source/External/imgui/imgui.h>
#include <Source/External/imgui/imgui_impl_glfw.h>
#include <Source/External/imgui/imgui_impl_opengl3.h>

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
	Texture m_Texture("Source/Textures/SM_Pillar_NN_01a_lambert2_Normal.png");
	m_Texture.Bind(0);
	m_Shader.SetUniform1i("u_Texture", 0);

	// Vertex Buffer Layout
	VertexBufferLayout VBL;
	VBL.PushData<float>(3);
	VBL.PushData<float>(2);
	VA.AddBuffer(VB, VBL);

	// Initializing ImGui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init((char*) glGetString(330));

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

	while (!glfwWindowShouldClose(window))
	{
		// Rotate
		Model = glm::rotate(Model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_Shader.SetUniformMat4f("m_MVP", Projection * View * Model);

		// Bind
		VA.Bind();
		IB.Bind();

		// ImGui 
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");

			ImGui::Text("This is some useful text.");
			ImGui::Checkbox("Demo Window", &show_demo_window);
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
			ImGui::ColorEdit3("clear color", (float*)&clear_color);

			if (ImGui::Button("Button"))
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Renderer
		Renderer m_Renderer(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		// Rendering ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// UnBind
	VB.UnBind();
	IB.UnBind();
	VA.UnBind();
	m_Shader.UnBind();
	m_Texture.UnBind();

	// Destroy ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Destroy, Terminate and Exit
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}