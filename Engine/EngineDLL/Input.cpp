#include "Input.h"

Input * Input::instance = NULL;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

Input::Input()
{
}

bool Input::isInput(int key)
{
	int state = glfwGetKey((GLFWwindow*)window->GetContext(), key);
	if (state == GLFW_PRESS) return true;
	return false;
}

void Input::SetWindowContext(Window* window)
{
	this->window = window;
	glfwSetKeyCallback((GLFWwindow*)this->window->GetContext(), KeyCallback);
}

void Input::PollEvents()
{
	glfwPollEvents();
}