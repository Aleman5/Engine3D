#pragma once

#include "Exports.h"
#include "GL\glew.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include "GLFW\glfw3.h"

class ENGINEDLL_API Defs
{
	static Defs *instance;
	double lastTime;	// Previous frame

	Defs();
public:
	double deltaTime;	// Delta time
	double currentTime; // Actual Time

	void UpdateDeltaTime()
	{
		currentTime = glfwGetTime();			// Save the actual time
		deltaTime = currentTime - lastTime;	// Make a difference btw the actualFrame and the lastFrame
		lastTime = currentTime;				// Save the lastFrame with the actual time
	}

	static Defs* getInstance()
	{
		if (instance == NULL) instance = new Defs();
		return instance;
	}
	Defs (Defs const&) = delete;
	void operator = (Defs const&) = delete;
};