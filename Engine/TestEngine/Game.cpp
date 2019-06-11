#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::OnStart()
{
#pragma region Version without Scene Graph
	GetRenderer()->SetProjPersp(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 1000.0f);

	camera = new Camera(GetRenderer());

	input = Input::getInstance();
	input->SetWindowContext(GetWindow());

	CollisionManager* cmgr = CollisionManager::getInstance();

	cmgr->SetRelation(Character, Enemy);
	cmgr->SetRelation(Character, Wall);
	cmgr->SetRelation(Enemy, Wall);

	speed = 2.0f;
	gameState = CONTINUE;

	material = new Material();
	material->LoadShader("Shaders\\SimpleVertexShader.vertexshader"
		, "Shaders\\SimpleFragmentShader.fragmentshader");

	matTexture = new Material();
	matTexture->LoadShader("Shaders\\TextureVertexShader.vertexshader"
		, "Shaders\\TextureFragmentShader.fragmentshader");

	matMesh = new Material();
	matMesh->LoadShader("Shaders\\MeshVertexShader.vertexshader"
		, "Shaders\\MeshFragmentShader.fragmentshader");

	M4A1 = new Mesh(GetRenderer(), matTexture, Default, "M4A1\\M4A1.FBX", "M4A1\\M4A1Tex.bmp");
	M4A1->Teleport(0.0f, -100.0f, 0.0f);

	spider = new Mesh(GetRenderer(), matTexture, Default, "spider.obj", "SpiderTex.bmp");
	spider->Teleport(-50.0f, -50.0f, 50.0f);

	thorHammer = new Mesh(GetRenderer(), matTexture, Default, "ThorHammer\\thorhammer.obj", "ThorHammer\\thorcolor.bmp");
	thorHammer->Teleport(-50.0f, 50.0f, 50.0f);
	thorHammer->RotateX(180.0f);
#pragma endregion

#pragma region Version with Scene Graph
	scene = new Node();
	SetScene(scene);
	nWeapon = new Node(scene);
	nWeapon->AddComponent(new Transform());

	nSpider = new Node(scene);
	nThorHammer = new Node(scene);
#pragma endregion


	return true;
}

bool Game::OnStop()
{
#pragma region Version without Scene Graph
	delete material;
	delete matTexture;
	delete matMesh;
	delete M4A1;
	delete spider;
	delete thorHammer;
#pragma endregion

#pragma region Version with Scene Graph
	delete nWeapon;
	delete nSpider;
	delete nThorHammer;
#pragma endregion
	return true;
}

bool Game::OnUpdate()
{
	switch (gameState)
	{
	case CONTINUE:
	{
		float cameraSpeed = 2.0f;

		if (input->isInput(GLFW_KEY_Q))
			camera->Walk(0.0f, 5.0f * cameraSpeed * Defs::getInstance()->deltaTime);
		if (input->isInput(GLFW_KEY_A))
			camera->Walk(0.0f, 5.0f * cameraSpeed * -Defs::getInstance()->deltaTime);
		if (input->isInput(GLFW_KEY_W))
			camera->Walk(5.0f * cameraSpeed * Defs::getInstance()->deltaTime, 0.0f);
		if (input->isInput(GLFW_KEY_S))
			camera->Walk(5.0f * cameraSpeed * -Defs::getInstance()->deltaTime, 0.0f);
		if (input->isInput(GLFW_KEY_E))
			camera->Roll(cameraSpeed * Defs::getInstance()->deltaTime);
		if (input->isInput(GLFW_KEY_D))
			camera->Roll(cameraSpeed * -Defs::getInstance()->deltaTime);
		if (input->isInput(GLFW_KEY_R))
			camera->Pitch(cameraSpeed * Defs::getInstance()->deltaTime);
		if (input->isInput(GLFW_KEY_F))
			camera->Pitch(cameraSpeed * -Defs::getInstance()->deltaTime);
		if (input->isInput(GLFW_KEY_T))
			camera->Yaw(cameraSpeed * Defs::getInstance()->deltaTime);
		if (input->isInput(GLFW_KEY_G))
			camera->Yaw(cameraSpeed * -Defs::getInstance()->deltaTime);
	}
		break;
	case WIN:
	{

	}
		break;
	case LOSE:
	{

	}
		break;
	}
#pragma region Version without Scene Graph
	M4A1->RotateY(speed * Defs::getInstance()->deltaTime);
	spider->RotateY(speed * Defs::getInstance()->deltaTime);
	thorHammer->RotateY(speed * Defs::getInstance()->deltaTime);

	M4A1->Update();
	spider->Update();
	thorHammer->Update();
#pragma endregion

#pragma region Version with Scene Graph

#pragma endregion

	if (gameState == 0)
		CollisionManager::getInstance()->DetectCollisions();

	input->PollEvents();

	return true;
}

bool Game::OnDraw()
{
#pragma region Version without Scene Graph
	M4A1->Draw();
	spider->Draw();
	thorHammer->Draw();
#pragma endregion

#pragma region Version with Scene Graph
	
#pragma endregion

	return true;
}

void Game::Restart()
{
	gameState = CONTINUE;
}