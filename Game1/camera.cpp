#include "camera.hpp"
#include "input.hpp"
#include <glm/gtx/transform.inl>
#include <glm/gtc/matrix_transform.inl>

Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, const int& width, const int& height)
	: _width(width), _height(height)
{
	_position = position;
	_direction = glm::normalize(target - position);
	_worldUp = up;
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
	
	
	cameraData.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(_width) / static_cast<float>(_height), _nearPlane, _farPlane);
	cameraData.invProj = glm::inverse(cameraData.proj);
	cameraData.view = glm::lookAt(_position, _position + _direction, _worldUp);
	cameraData.invView = glm::inverse(cameraData.view);
	cameraData.viewProj = cameraData.proj * cameraData.view;
	cameraData.invViewProj = glm::inverse(cameraData.viewProj);
	cameraData.wsPosition = glm::vec4(_position, 1.f);
}

void Camera::resize(const int& width, const int& height)
{
	_width = width; _height = height;
}
