#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <glm/vec2.hpp>
#include <glm/gtx/transform.hpp>

#include "Exports.h"
#include "Definitions.h"
#include "Window.h"


using namespace glm;

enum class Axis
{
	HORIZONTAL,
	VERTICAL
};

class ENGINEDLL_API Input
{
	static Input *instance;

	const float MOUSE_SENSITIVITY = 100.0f;

	Window* window;

	bool firstMouseMovement;
	vec2 lastMousePosition;
	vec2 currentMousePosition;

public:
	bool isInput(int key);

	void SetWindowContext(Window* window);
	void PollEvents();
	void ShowCursor();
	void HideCursor();
	float GetAxis(Axis inputAxis);

	inline bool FirstMouseMovement() { return firstMouseMovement; }
	inline vec2 GetLastMousePosition() { return lastMousePosition; }
	inline vec2 GetCurrentMousePosition() { return currentMousePosition; }

	inline void SetFirstMouseMovement(bool value) { firstMouseMovement = value; }
	inline void SetLastMousePosition(vec2 mousePosition) { lastMousePosition = mousePosition; }
	inline void SetCurrentMousePosition(vec2 mousePosition) { currentMousePosition = mousePosition; }

	static Input* getInstance()
	{
		if (instance == NULL) instance = new Input();
		return instance;
	}
	Input(Input const&) = delete;
	void operator = (Input const&) = delete;
private:
	Input();
};

