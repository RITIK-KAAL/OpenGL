#include "Renderer.h"
#include <D:/OpenGL/Source/deps/glad/gl.h>
#include <D:/OpenGL/Dependencies/include/GLFW/glfw3.h>

Renderer::Renderer(const unsigned int Mode, const unsigned int Count, const unsigned int Type, const void* Indices)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawElements(Mode, Count, Type, Indices);
}
