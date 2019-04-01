#pragma once

#include "Exports.h"
#include "GL\glew.h"

#define GLM_ENABLE_EXPERIMENTAL

#include "GLFW\glfw3.h"

class ENGINEDLL_API Defs
{
	static Defs *instance;
	double currentFrame; // Actual Time
	double lastFrame;	// Previous frame

	Defs();
public:
	double deltaTime;	// Delta time

	void UpdateDeltaTime()
	{
		currentFrame = glfwGetTime();			// Save the actual time
		deltaTime = currentFrame - lastFrame;	// Make a difference btw the actualFrame and the lastFrame
		lastFrame = currentFrame;				// Save the lastFrame with the actual time
	}

	static Defs* getInstance()
	{
		if (instance == NULL) instance = new Defs();
		return instance;
	}
	Defs (Defs const&) = delete;
	void operator = (Defs const&) = delete;
};