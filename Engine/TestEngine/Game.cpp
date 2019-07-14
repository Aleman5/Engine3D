#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::OnStart()
{
	input = Input::getInstance();
	input->SetWindowContext(GetWindow());

	CollisionManager* cmgr = CollisionManager::getInstance();

	cmgr->SetRelation(Character, Enemy);
	cmgr->SetRelation(Character, Wall);
	cmgr->SetRelation(Enemy, Wall);

	speed = 2.0f;
	gameState = CONTINUE;

	material = new Material();
	material->LoadShader(SIMPLE_VERTEX_SHADER, SIMPLE_FRAGMENT_SHADER);

	matTexture = new Material();
	matTexture->LoadShader(TEXTURE_VERTEX_SHADER, TEXTURE_FRAGMENT_SHADER);

	scene = new Node("Scene");
	SetScene(scene);

	nCamera = new Node("CameraHolder", scene);
	camera = (Camera*)nCamera->AddComponent(new Camera(matTexture));
	camera->Teleport(0.0f, 0.0f, -20.0f);
	camera->SetAsMainCamera();

	nDebugCamera = new Node("DebugCameraHolder", scene);
	debugCamera = (Camera*)nDebugCamera->AddComponent(new Camera(matTexture));
	debugCamera->Teleport(0.0f, 140.0f, -70.0f);
	debugCamera->Pitch(45.0f);
	debugCamera->SetSpeed(200.0f);
	debugCamera->SetAsMainDebugCamera();

	//scene->ActivateCameraDebugMode();

	nMeshes = new Node("Meshes", scene);

	nWeapon = new Node("M4A1", nMeshes);
	nWeapon->transform->SetLayer(Character);
	nWeapon->transform->Teleport(0.0f, -100.0f, 0.0f);
	ModelImporter::getInstance()->Load(nWeapon, M4A1_PATH, M4A1_TEXTURE_PATH, matTexture);
	Mesh* weaponMesh = (Mesh*)nWeapon->GetComponent("Mesh");
	weaponMesh->ActivateDebugMode();

	nSpider = new Node("Spider", nMeshes);
	nSpider->transform->SetLayer(Enemy);
	nSpider->transform->Teleport(-50.0f, -50.0f, 50.0f);
	ModelImporter::getInstance()->Load(nSpider, SPIDER_PATH, SPIDER_TEXTURE_PATH, matTexture);

	/*nThorHammer = new Node("Thor Hammer", nMeshes);
	nThorHammer->transform->SetLayer(Wall);
	nThorHammer->transform->Teleport(-50.0f, 50.0f, 50.0f);
	nThorHammer->AddComponent(new Mesh(THOR_HAMMER_PATH, THOR_HAMMER_TEXTURE_PATH));
	nThorHammer->DesactivateNode();*/

	nHelicopter = new Node("Helicopter", nMeshes);
	nHelicopter->transform->SetLayer(Character);
	nHelicopter->transform->Teleport(20.0f, 20.0f, 20.0f);
	nHelicopter->transform->Scale(0.01f, 0.01f, 0.01f);
	ModelImporter::getInstance()->Load(nHelicopter, HELICOPTER_PATH, HELICOPTER_TEXTURE_PATH, matTexture);

	/*nNanosuit = new Node("Nanosuit", nMeshes);
	nNanosuit->transform->SetLayer(Character);
	nNanosuit->transform->Teleport(60.0f, 20.0f, 20.0f);
	nNanosuit->AddComponent(new Mesh(matTexture, NANOSUIT_PATH, NANOSUIT_TEXTURE_PATH));*/

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
		float cameraSpeed = 50.0f;

		if (input->isInput(GLFW_KEY_W))
			camera->WalkFront(ADVANCE);
		if (input->isInput(GLFW_KEY_S))
			camera->WalkFront(BACK);
		if (input->isInput(GLFW_KEY_A))
			camera->WalkSideWays(ADVANCE);
		if (input->isInput(GLFW_KEY_D))
			camera->WalkSideWays(BACK);
		if (input->isInput(GLFW_KEY_Q))
			camera->Rise(ADVANCE);
		if (input->isInput(GLFW_KEY_E))
			camera->Rise(BACK);

		if (input->isInput(GLFW_KEY_R))
			camera->Roll(cameraSpeed * Defs::getInstance()->deltaTime);
		if (input->isInput(GLFW_KEY_F))
			camera->Roll(cameraSpeed * -Defs::getInstance()->deltaTime);
		if (input->isInput(GLFW_KEY_T))
			camera->Pitch(cameraSpeed * Defs::getInstance()->deltaTime);
		if (input->isInput(GLFW_KEY_G))
			camera->Pitch(cameraSpeed * -Defs::getInstance()->deltaTime);
		if (input->isInput(GLFW_KEY_Y))
			camera->Yaw(cameraSpeed * Defs::getInstance()->deltaTime);
		if (input->isInput(GLFW_KEY_H))
			camera->Yaw(cameraSpeed * -Defs::getInstance()->deltaTime);

		if ((input->isInput(GLFW_KEY_X)))
		{
			camera->SetAsMainCamera();
		}

		if ((input->isInput(GLFW_KEY_C)))
		{
			camera->RemoveAsMainCamera();
		}

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

	//nWeapon->transform->RotateY(speed * 30.0f * Defs::getInstance()->deltaTime);
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