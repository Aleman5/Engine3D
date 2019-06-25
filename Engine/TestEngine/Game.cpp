#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::OnStart()
{
	GetRenderer()->SetProjPersp(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 1000.0f);

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

	scene = new Node("Scene");
	SetScene(scene);

	nCamera = new Node("CameraHolder", scene);
	camera = (Camera*)nCamera->AddComponent(new Camera());

	nWeapon = new Node("M4A1", scene);
	nWeapon->transform->SetLayer(Character);
	nWeapon->transform->Teleport(0.0f, -100.0f, 0.0f);
	nWeapon->AddComponent(new Mesh(matTexture, "M4A1\\M4A1.FBX", "M4A1\\M4A1Tex.bmp"));

	nSpider = new Node("Spider", nWeapon);
	nSpider->transform->SetLayer(Enemy);
	nSpider->transform->Teleport(-50.0f, -50.0f, 50.0f);
	nSpider->AddComponent(new Mesh(matTexture, "spider.obj", "SpiderTex.bmp"));

	nThorHammer = new Node("Thor Hammer", scene);
	nThorHammer->transform->SetLayer(Wall);
	nThorHammer->transform->Teleport(-50.0f, 50.0f, 50.0f);
	nThorHammer->AddComponent(new Mesh(matTexture, "ThorHammer\\thorhammer.obj", "ThorHammer\\thorcolor.bmp"));

	return true;
}

bool Game::OnStop()
{
	delete material;
	delete matTexture;
	delete matMesh;

	delete scene;

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

		if ((input->isInput(GLFW_KEY_V)))
		{
			if (scene->IsActive())
				scene->DesactivateNode();
			else
				scene->ActivateNode();
		}

		if ((input->isInput(GLFW_KEY_B)))
		{
			if (nWeapon->IsActive())
				nWeapon->DesactivateNode();
			else
				nWeapon->ActivateNode();
		}

		if ((input->isInput(GLFW_KEY_N)))
		{
			if (nSpider->IsActive())
				nSpider->DesactivateNode();
			else
				nSpider->ActivateNode();
		}

		if ((input->isInput(GLFW_KEY_M)))
		{
			if (nThorHammer->IsActive())
				nThorHammer->DesactivateNode();
			else
				nThorHammer->ActivateNode();
		}

		if ((input->isInput(GLFW_KEY_J)))
		{
			if (nWeapon->GetComponent("Mesh"))
				nWeapon->RemoveComponent("Mesh");
		}
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

	nWeapon->transform->RotateY(speed * 30.0f * Defs::getInstance()->deltaTime);
	//nSpider->transform->RotateY(speed * Defs::getInstance()->deltaTime);
	//nThorHammer->transform->RotateY(speed * Defs::getInstance()->deltaTime);

	if (gameState == 0)
		CollisionManager::getInstance()->DetectCollisions();

	input->PollEvents();

	return true;
}

bool Game::OnDraw()
{
	return true;
}

void Game::Restart()
{
	gameState = CONTINUE;
}