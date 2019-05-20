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

	camera = new Camera(GetRenderer());
	
	input = Input::getInstance();
	input->SetWindowContext(GetWindow());

	CollisionManager* cmgr = CollisionManager::getInstance();
	
	cmgr->SetRelation(Character, Enemy);
	cmgr->SetRelation(Character, Wall);
	cmgr->SetRelation(Enemy, Wall);

	speed = 100.0f;
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

	//GetRenderer()->MoveCamera(glm::vec3(0.0f, 64.0f, 0.0f));

	M4A1 = new Mesh(GetRenderer(), matTexture, Default, "M4A1\\M4A1.FBX", "M4A1\\M4A1Tex.bmp");
	M4A1->Teleport(0.0f, -100.0f, 0.0f);

	spider = new Mesh(GetRenderer(), matTexture, Default, "spider.obj", "SpiderTex.bmp");
	spider->Teleport(-50.0f, -50.0f, 50.0f);

	nanosuit = new Mesh(GetRenderer(), matTexture, Default, "Nanosuit\\nanosuit.obj", "M4A1\\M4A1Tex.bmp");
	nanosuit->Teleport(-50.0f, -50.0f, 50.0f);

	return true;
}

bool Game::OnStop()
{
	delete material;
	delete matTexture;
	delete M4A1;
	delete spider;
	delete nanosuit;

	return true;
}

bool Game::OnUpdate()
{
	switch (gameState)
	{
	case CONTINUE:
	{
		vec2 move = vec2(speed * Defs::getInstance()->deltaTime, 0.0f);

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

	M4A1->Update();
	spider->Update();
	nanosuit->Update();

	if (gameState == 0)
		CollisionManager::getInstance()->DetectCollisions();

	input->PollEvents();

	return true;
}

bool Game::OnDraw()
{
	M4A1->Draw();
	spider->Draw();
	nanosuit->Draw();

	return true;
}

void Game::Restart()
{
	gameState = CONTINUE;
}