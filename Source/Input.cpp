#include "Input.h"
#include <Dependencies/include/GLFW/glfw3.h>

void Input::MoveRight(glm::vec3 Position, float Speed) const
{
	Position = m_Position; 
	Speed = m_Speed;

	Position += Position * Speed;
}
