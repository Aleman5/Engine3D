#pragma once

#include "Exports.h"
#include "GL\glew.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include "GLFW\glfw3.h"

using namespace glm;

class ENGINEDLL_API Defs
{
	static Defs *instance;
	double lastTime;	// Previous frame

	Defs();
public:
	double deltaTime;	// Delta time
	double currentTime; // Actual Time
	vec3 lightDirection = vec3(-5.0f, 4.0f, 0.0f);// Actual Light direction

	void UpdateDeltaTime()
	{
		currentTime = glfwGetTime();		// Save the actual time
		deltaTime = currentTime - lastTime;	// Make a difference btw the actualFrame and the lastFrame
		lastTime = currentTime;				// Save the lastFrame with the actual time

		//lightDirection = vec3(sin(currentTime * 2.0f) * 6.0f, 4.0f, sin(currentTime * 1.25f) * 5.0f);
	}

	static Defs* getInstance()
	{
		if (instance == NULL) instance = new Defs();
		return instance;
	}
	Defs (Defs const&) = delete;
	void operator = (Defs const&) = delete;
};