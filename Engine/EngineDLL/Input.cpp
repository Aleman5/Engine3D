#include "Input.h"
#include <GLFW/glfw3.h>

Input * Input::instance = NULL;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

void MousePosCallback(GLFWwindow* window, double mouseX, double mouseY)
{
	Input* input = Input::getInstance();

	if (input->FirstMouseMovement())
	{
		input->SetLastMousePosition(vec2((float)mouseX, (float)mouseY));
		input->SetFirstMouseMovement(false);
	}
	input->SetCurrentMousePosition(vec2((float)mouseX, (float)mouseY));
}

Input::Input()
	: firstMouseMovement(true), currentMousePosition(vec2(0.0f, 0.0f)), lastMousePosition(vec2(0.0f, 0.0f))
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
	glfwSetCursorPosCallback((GLFWwindow*)this->window->GetContext(), MousePosCallback);
}

void Input::PollEvents()
{
	glfwPollEvents();
}

float Input::GetAxis(Axis inputAxis)
{
	float value = 0.0f;
	float delta = 0.0f;

	if (inputAxis == Axis::HORIZONTAL)
	{
		delta = currentMousePosition.x - lastMousePosition.x;
		if (delta != 0.0f)
		{
			value = log2(1.0f + abs(delta) / window->GetWidth());
			lastMousePosition.x = currentMousePosition.x;
		}
	}
	else
	{
		delta = lastMousePosition.y - currentMousePosition.y;
		if (delta != 0.0f)
		{
			value = log2(1.0f + abs(delta) / window->GetHeight());;
			lastMousePosition.y = currentMousePosition.y;
		}
	}

	value = -sign(delta) * value * MOUSE_SENSITIVITY;

	return value;
}

void Input::ShowCursor()
{
	glfwSetInputMode((GLFWwindow*)window->GetContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Input::HideCursor()
{
	glfwSetInputMode((GLFWwindow*)window->GetContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}