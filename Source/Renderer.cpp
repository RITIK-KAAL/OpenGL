#include "Renderer.h"
#include <Source/deps/glad/gl.h>
#include <Dependencies/include/GLFW/glfw3.h>

Renderer::Renderer(const unsigned int Mode, const unsigned int Count, const unsigned int Type, const void* Indices)
{
	glDrawElements(Mode, Count, Type, Indices);
}
