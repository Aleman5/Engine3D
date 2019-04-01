#include "GameBase.h"

GameBase::GameBase()
{
}

GameBase::~GameBase()
{
}

/*void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}*/

bool GameBase::Start(int width, int height, const char* windowMe)
{
	Defs* defs = Defs::getInstance();

	Defs::getInstance()->UpdateDeltaTime();

	currentFrame = 0.0f;
	lastFrame = 0.0f;

	window = new Window();
	if (!window->Start(width, height, windowMe))
	{
		delete window;
		return false;
	}

	renderer = new Renderer();
	if (!renderer->Start(window))
	{
		delete renderer;
		return false;
	}

	//glfwSetKeyCallback((GLFWwindow*)window->GetContext(), KeyCallback);

	if (!OnStart())
	{
		return false;
	}
	return true;
}

bool GameBase::Stop()
{
	OnStop();

	renderer->Stop();
	delete renderer;

	window->Stop();
	delete window;

	return true;
}

void GameBase::Loop()
{
	renderer->SetClearColor(0.3f, 0.3f, 0.3f, 0.0); // Dark Gray

	bool state = true;
	while (!window->ShouldClose() && state)
	{
		renderer->ClearScreen();

		Defs::getInstance()->UpdateDeltaTime();

		state = OnUpdate();
		state = OnDraw();

		renderer->SwapBuffers();
	}
}

/*bool GameBase::input(int key)
{
	int state = glfwGetKey((GLFWwindow*)window->GetContext(), key);
	if (state == GLFW_PRESS) return true;
	return false;
}*/

Window* GameBase::GetWindow()
{
	return window;
}

Renderer* GameBase::GetRenderer()
{
	return renderer;
}