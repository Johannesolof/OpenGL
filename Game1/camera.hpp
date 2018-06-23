#pragma once

#include <glm/glm.hpp>

class Input;

class Camera
{
public:
	Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, const int& width, const int& height);

	void update(const Input& input, float deltaTime);
	void resize(const int& width, const int& height);

	struct cameraData_t
	{
		glm::mat4 proj;
		glm::mat4 invProj;
		glm::mat4 view;
		glm::mat4 invView;
		glm::mat4 viewProj;
		glm::mat4 invViewProj;
		glm::vec4 wsPosition;	
	} cameraData{};

	

private:
	glm::vec3 _position;
	glm::vec3 _direction;
	glm::vec3 _worldUp;
	
	int _width;
	int _height;
	float _farPlane = 100.f;
	float _nearPlane = .1f;
	
	float _moveSpeed = 10.0f;
	float _rotationSpeed = 10.f;

	float _pitch;
	float _yaw;
	float _roll;
};
