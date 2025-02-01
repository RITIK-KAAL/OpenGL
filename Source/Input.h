#pragma once

#include <Source/External/glm/glm.hpp>
#include <string>

class Input
{
private:

public:

	void MoveRight(glm::vec3& Position, float Speed, std::string Axis) const;

};
