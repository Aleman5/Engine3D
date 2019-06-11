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

	if(nScene)
		delete nScene;

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
		if (nScene)
			nScene->Update();
		state = OnDraw();
		if (nScene)
			nScene->Draw();

		renderer->SwapBuffers();
	}
}

Window* GameBase::GetWindow()
{
	return window;
}

Renderer* GameBase::GetRenderer()
{
	return renderer;
}

Node* GameBase::GetActualScene()
{
	return nScene;
}

void GameBase::SetScene(Node* scene)
{
	nScene = scene;
}