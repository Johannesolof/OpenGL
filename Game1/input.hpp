#pragma once
#include <string>
#include <unordered_map>
#include <GLFW/glfw3.h>
#include <memory>

struct MouseState
{
	double xpos = 0, ypos = 0, prevXpos = 0, prevYpos = 0;
	float dx = 0.f, dy = 0.f;
	bool pressed[5] = { false };
};

class Input
{
public:
	explicit Input(GLFWwindow* window);
	~Input();

	void update();

	bool pressed(const std::string& name) const;
	const MouseState& getMouseState() const;

private:
	struct Key
	{
		std::string name;
		unsigned int code;
		bool pressed = false;
	};

	GLFWwindow * _window;
	std::vector<Key> _keyMapping;
	std::unique_ptr<MouseState> _mouseState;
};
