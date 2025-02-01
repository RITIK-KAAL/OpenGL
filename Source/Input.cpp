#include "Input.h"
#include <Dependencies/include/GLFW/glfw3.h>

void Input::MoveRight(glm::vec3& Position, float Speed, std::string Axis) const
{
	if (Axis == "Z")
	{
		Position.z += Speed;
	}
	else if (Axis == "X")
	{
		Position.x += Speed;
	}
}
