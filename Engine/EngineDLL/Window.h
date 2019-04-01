#pragma once

#include <iostream>
#include <string>

using namespace std;

class Window
{
	void* window;		// Window
	int windowWidth;	// Window width
	int windowHeight;	// Window height
	string windowName;	// Window name

public:
	bool Start(int width,		// Width of the Window
		int height,				// Height of the Window
		const char* windowMe	// Name of the Window
	);
	bool Stop();
	bool ShouldClose();

	void* GetContext() { return window;		  }; // Returns a pointer to void
	int GetWidth()	   { return windowWidth;  }; // Returns the Width of the Window
	int GetHeight()    { return windowHeight; }; // Returns the Height of the Window

	Window();
	~Window();
};

