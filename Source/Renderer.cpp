#include "Renderer.h"
#include <Source/deps/glad/gl.h>
#include <Dependencies/include/GLFW/glfw3.h>

Renderer::Renderer(const unsigned int Mode, const unsigned int Count, const unsigned int Type, const void* Indices)
{
	// Clear Screen and Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawElements(Mode, Count, Type, Indices);
}
