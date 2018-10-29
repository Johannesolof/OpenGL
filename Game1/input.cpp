#include "input.hpp"
#include <functional>

Input::Input(GLFWwindow *window) : _window(window)
{
	_keyMapping.push_back({"forward", GLFW_KEY_W});
	_keyMapping.push_back({"backward", GLFW_KEY_S});
	_keyMapping.push_back({"right", GLFW_KEY_D});
	_keyMapping.push_back({"left", GLFW_KEY_A});
	_keyMapping.push_back({"upward", GLFW_KEY_E});
	_keyMapping.push_back({"downward", GLFW_KEY_Q});
	_keyMapping.push_back({"quit", GLFW_KEY_ESCAPE});
	_mouseState = std::make_unique<MouseState>();
}

Input::~Input()
= default;

void Input::update()
{
	_mouseState->prevXpos = _mouseState->xpos;
	_mouseState->prevYpos = _mouseState->ypos;
	glfwGetCursorPos(_window, &_mouseState->xpos, &_mouseState->ypos);
	_mouseState->dx = static_cast<float>(_mouseState->xpos - _mouseState->prevXpos);
	_mouseState->dy = static_cast<float>(_mouseState->ypos - _mouseState->prevYpos);

	for (int i = 0; i < 5; ++i)
	{
		const int buttonState = glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_1 + i);
		_mouseState->pressed[i] = buttonState == GLFW_PRESS;
	}

	for (auto& keyPair : _keyMapping)
	{
		keyPair.pressed = glfwGetKey(_window, keyPair.code) == GLFW_PRESS;
	}
}

bool Input::pressed(const std::string& name) const
{
	for (auto keyPair : _keyMapping)
	{
		if(keyPair.name == name)
			return keyPair.pressed;
	}
	printf("Input: %s is not mapped", name.c_str());
	return false;
}

const MouseState& Input::getMouseState() const
{
	return *_mouseState;
}