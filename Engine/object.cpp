#include "object.hpp"

glm::vec3 Object::getPosition() const
{
	return glm::vec3(transform[3]);
}
