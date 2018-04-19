#pragma once

#include <glm/glm.hpp>

class Input;

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up);

	glm::mat4 getViewMatrix() const;
	glm::vec3 getPosition() const;

	void update(const Input& input, float deltaTime);

	float farPlane = 100.f;
	float nearPlane = .1f;
private:
	glm::vec3 _position;
	glm::vec3 _direction;
	glm::vec3 _worldUp;

	float _moveSpeed = 10.0f;
	float _rotationSpeed = 10.f;


	float _pitch;
	float _yaw;
	float _roll;
};
