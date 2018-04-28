#pragma once

#include <glm/glm.hpp>

class Object
{
public:
	glm::mat4 transform;

	inline glm::vec3 getPosition() const;
};