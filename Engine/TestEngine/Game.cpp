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
	input->HideCursor();

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
	camera = (Camera*)nCamera->AddComponent(new Camera(matTexture, true));
	camera->Teleport(0.0f, 0.0f, -50.0f);
	camera->SetAsMainCamera();

	nDebugCamera = new Node("DebugCameraHolder", scene);
	debugCamera = (Camera*)nDebugCamera->AddComponent(new Camera(matTexture, true));
	debugCamera->Teleport(0.0f, 140.0f, -70.0f);
	debugCamera->Pitch(45.0f);
	debugCamera->SetSpeed(200.0f);
	debugCamera->SetAsMainDebugCamera();

	//scene->ActivateCameraDebugMode();

	nObjects = new Node("Objects", scene);

	nWeapon = new Node("M4A1", nObjects);
	nWeapon->transform->SetLayer(Character);
	nWeapon->transform->Teleport(0.0f, -30.0f, 0.0f);

	nWeaponMesh = new Node("M4A1Mesh", nWeapon);
	nWeaponMesh->transform->Scale(0.4f, 0.4f, 0.4f);
	ModelImporter::getInstance()->Load(nWeaponMesh, M4A1_PATH, M4A1_TEXTURE_PATH, matTexture);
	//nWeaponMesh->ActivateMeshDebugMode();

	nSpider = new Node("Spider", nWeapon);
	nSpider->transform->SetLayer(Enemy);
	nSpider->transform->Teleport(-50.0f, -30.0f, 50.0f);

	nSpiderMesh = new Node("SpiderMesh", nSpider);
	nSpiderMesh->transform->Scale(0.4f, 0.4f, 0.4f);
	ModelImporter::getInstance()->Load(nSpiderMesh, SPIDER_PATH, SPIDER_TEXTURE_PATH, matTexture);
	//nSpiderMesh->ActivateMeshDebugMode();

	nHelicopter = new Node("Helicopter", nObjects);
	nHelicopter->transform->SetLayer(Character);
	nHelicopter->transform->Teleport(20.0f, 10.0f, 20.0f);
	
	nHelicopterMesh = new Node("HelicopterMesh", nHelicopter);
	nHelicopterMesh->transform->Scale(0.01f, 0.01f, 0.01f);
	ModelImporter::getInstance()->Load(nHelicopterMesh, HELICOPTER_PATH, HELICOPTER_TEXTURE_PATH, matTexture);
	//nHelicopterMesh->ActivateMeshDebugMode();

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

		/*if (input->isInput(GLFW_KEY_R))
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
			camera->Yaw(cameraSpeed * -Defs::getInstance()->deltaTime);*/

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

		if (input->isInput(GLFW_KEY_ESCAPE))
		{
			return false;
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

	//nObjects->transform->RotateX(speed * 30.0f * Defs::getInstance()->deltaTime);
	//nWeapon->transform->RotateZ(speed * 10.0f * Defs::getInstance()->deltaTime);

	//nWeapon->transform->RotateY(speed * 30.0f * Defs::getInstance()->deltaTime);
	//nSpider->transform->RotateY(speed * Defs::getInstance()->deltaTime);
	//nThorHammer->transform->RotateY(speed * Defs::getInstance()->deltaTime);

	if (gameState == 0)
	{
		CollisionManager::getInstance()->DetectCollisions();
		input->PollEvents();
	}

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