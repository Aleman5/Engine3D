#include "Window.h"

#include <stdio.h>
#include <stdlib.h>
#include "glm\glm.hpp"
#include "GL\glew.h"
#include "GLFW/glfw3.h"

using namespace glm;

Window::Window()
{
}
Window::~Window()
{
}

bool Window::Start(int width, int height, const char* windowMe)
{
	windowWidth = width;
	windowHeight = height;
	windowName = windowMe;

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		glfwTerminate();
		return false;
	}

	window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return false;
	}

	return true;
}

bool Window::Stop()
{
	if (window)
		glfwDestroyWindow((GLFWwindow*) window);

	window = NULL;

	glfwTerminate();

	return true;
}

bool Window::ShouldClose()
{
	if (window)
		return glfwWindowShouldClose((GLFWwindow*)window);
	return true;
}