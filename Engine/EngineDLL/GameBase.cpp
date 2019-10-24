#include "GameBase.h"
#include "GlobalDefs.h"

GameBase::GameBase()
{
}

GameBase::~GameBase()
{
}

bool GameBase::Start(int width, int height, const char* windowMe)
{
	Defs* defs = Defs::getInstance();

	Defs::getInstance()->UpdateDeltaTime();

	physicsManager = PhysicsManager::getInstance();
	physicsManager->Start(vec3(0.0f, DEFAULT_GRAVITY_Y, 0.0f), 8u);

	currentFrame = 0.0f;
	lastFrame = 0.0f;

	window = new Window();
	if (!window->Start(width, height, windowMe))
	{
		delete window;
		return false;
	}

	renderer = Renderer::getInstance();
	if (!renderer->Start(window))
	{
		return false;
	}

	if (!OnStart())
	{
		return false;
	}

	physicsManager->PrepareMaterial();

	return true;
}

bool GameBase::Stop()
{
	OnStop();
	nScene = NULL;

	physicsManager->Stop();
	renderer->Stop();

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
		if (!state) break;
		if (nScene)
			nScene->Update();

		physicsManager->Simulate();
		physicsManager->FetchSimulationResults();

		if (!state) break;
		state = OnDraw();
		if (nScene)
		{
			renderer->MakeBSPClean(nScene);
			nScene->Draw();
		}

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