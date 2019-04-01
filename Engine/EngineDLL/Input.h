#pragma once

#include <iostream>
#include "Exports.h"
#include "Definitions.h"
#include "Window.h"

class ENGINEDLL_API Input
{
	static Input *instance;

	Window* window;

public:
	bool isInput(int key);

	void PollEvents();

	void SetWindowContext(Window* window);

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

