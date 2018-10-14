#pragma once

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include Imgui
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include "camera.hpp"
#include <memory>
#include "input.hpp"
#include "program.hpp"


struct FrameTime
{
	float deltaTime = 0;
	float currentTime = 0;
	float previousTime = 0;
	float deltaBuffer[10] = { 0 };
	uint8_t bufferIt = 0;

	double smoothDelta() // Not so smart
	{
		double tot = 0;
		for (auto d : deltaBuffer)
		{
			tot += d;
		}
		return tot / 10;
	}

	void update(float time)
	{
		currentTime = time;
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;
		deltaBuffer[bufferIt] = deltaTime;
		bufferIt = (bufferIt + 1) % 10;
	}
};

class App
{
public:
	explicit App(std::string name);
	~App();

	bool init();
	void run();

private:
	void drawGui();
	static void resize(GLFWwindow* window, int width, int height);

	std::string _name;
	int _width;
	int _height;
	GLFWwindow* _window = nullptr;
	glm::vec3 _worldUp = glm::vec3(0.f, 1.f, 0.f);
	std::shared_ptr<Camera> _camera;
	std::shared_ptr<Input> _input;
	glm::vec4 _clearColor;
	FrameTime _frameTime;
	je::Program _gbuffer;
};

