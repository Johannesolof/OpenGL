#include "camera.hpp"
#include <glm/glm.hpp>
#include "input.hpp"
#include <glm/gtx/transform.inl>
#include <glm/gtc/matrix_transform.inl>

Camera::Camera(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up)
{
	_position = position;
	_direction = glm::normalize(target- position);
	_worldUp = up;
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(_position, _position + _direction, _worldUp);
}

glm::vec3 Camera::getPosition() const
{
	return _position;
}

void Camera::update(const Input& input, float deltaTime)
{
	const MouseState ms = input.getMouseState();

	if (ms.pressed[1])
	{
		const glm::mat4 yawRot = glm::rotate(-ms.dx * deltaTime * _rotationSpeed, _worldUp);
		const glm::mat4 pitchRot = glm::rotate(-ms.dy * deltaTime * _rotationSpeed, glm::normalize(glm::cross(_direction, _worldUp)));
		_direction = glm::vec3(pitchRot * yawRot * glm::vec4(_direction, 0.f));
	}

	const glm::vec3 right = glm::cross(_direction, _worldUp);

	if (input.pressed("forward"))
	{
		_position += _direction * _moveSpeed * deltaTime;
	}
	if (input.pressed("backward"))
	{
		_position -= _direction * _moveSpeed * deltaTime;
	}
	if (input.pressed("right"))
	{
		_position += right * _moveSpeed * deltaTime;
	}
	if (input.pressed("left"))
	{
		_position -= right * _moveSpeed * deltaTime;
	}
	if (input.pressed("upward"))
	{
		_position += _worldUp * _moveSpeed * deltaTime;
	}
	if (input.pressed("downward"))
	{
		_position -= _worldUp * _moveSpeed * deltaTime;
	}

	

	//printf("Camera position x:%f y:%f z:%f\n", _position.x, _position.y, _position.z);
}
