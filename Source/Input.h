#pragma once

#include <Source/External/glm/glm.hpp>

class Input
{
private:

	glm::vec3 m_Position;
	float m_Speed = 0.0f;

public:

	void MoveRight(glm::vec3 Position, float Speed) const;

};
