#pragma once

#include <engine.hpp>
#include "camera.hpp"
#include <memory>
#include "input.hpp"
#include <memory>

struct FrameTime
{
	double deltaTime = 0;
	double currentTime = 0;
	double previousTime = 0;
	double deltaBuffer[10] = { 0 };
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

	void update(double time)
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
	App(std::string name);
	~App();

	void drawGui();
	void run();

private:
	std::string _name;
	int _width;
	int _height;
	GLFWwindow* _window = nullptr;
	std::shared_ptr<Camera> _camera;
	std::shared_ptr<Input> _input;
	glm::vec4 _clearColor;
	FrameTime _frameTime;
};

