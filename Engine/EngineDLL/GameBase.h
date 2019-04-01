#pragma once

#include "Exports.h"
#include "Definitions.h"
#include "Renderer.h"
#include "Window.h"
#include "Typedef.h"

#include "GLFW\glfw3.h"

class ENGINEDLL_API GameBase
{
	Window * window;	// Window reference
	Renderer* renderer; // Renderer reference
	
	double currentFrame;// Actual Time
	double lastFrame;	// Previous frame

protected:
	virtual bool OnStart() = 0;
	virtual bool OnStop() = 0;
	virtual bool OnUpdate() = 0;
	virtual bool OnDraw() = 0;

public:
	bool Start(
		int width,			// Width of the Window
		int height,			// Height of the Window
		const char* window	// Name of the Window
	);
	bool Stop();
	void Loop();
	//bool input(int key);
	Window* GetWindow();
	Renderer* GetRenderer();

	GameBase();
	~GameBase();
};

