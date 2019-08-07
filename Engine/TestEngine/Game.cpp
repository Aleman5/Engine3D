#include "Game.h"
#include "GlobalDefs.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::OnStart()
{
	/*
	input = Input::getInstance();
	input->SetWindowContext(GetWindow());
	input->HideCursor();

	CollisionManager* cmgr = CollisionManager::getInstance();

	cmgr->SetRelation(Character, Enemy);
	cmgr->SetRelation(Character, Wall);
	cmgr->SetRelation(Enemy, Wall);

	speed = 2.0f;
	gameState = CONTINUE;

	scene = new Node("Scene");
	SetScene(scene);

	nCamera = new Node("CameraHolder", scene);
	camera = (Camera*)nCamera->AddComponent(new Camera(true));
	camera->Teleport(0.0f, 0.0f, -50.0f);
	camera->SetAsMainCamera();

	nDebugCamera = new Node("DebugCameraHolder", scene);
	debugCamera = (Camera*)nDebugCamera->AddComponent(new Camera(true));
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
	ModelImporter::getInstance()->Load(nWeaponMesh, M4A1_PATH, M4A1_TEXTURE_PATH);
	//nWeaponMesh->ActivateMeshDebugMode();

	nSpider = new Node("Spider", nWeapon);
	nSpider->transform->SetLayer(Enemy);
	nSpider->transform->Teleport(-50.0f, -30.0f, 50.0f);

	nSpiderMesh = new Node("SpiderMesh", nSpider);
	nSpiderMesh->transform->Scale(0.4f, 0.4f, 0.4f);
	ModelImporter::getInstance()->Load(nSpiderMesh, SPIDER_PATH, SPIDER_TEXTURE_PATH);
	//nSpiderMesh->ActivateMeshDebugMode();

	nHelicopter = new Node("Helicopter", nObjects);
	nHelicopter->transform->SetLayer(Character);
	nHelicopter->transform->Teleport(20.0f, 10.0f, 20.0f);
	
	nHelicopterMesh = new Node("HelicopterMesh", nHelicopter);
	nHelicopterMesh->transform->Scale(0.01f, 0.01f, 0.01f);
	ModelImporter::getInstance()->Load(nHelicopterMesh, HELICOPTER_PATH, HELICOPTER_TEXTURE_PATH);
	//nHelicopterMesh->ActivateMeshDebugMode();
	*/
	
	input = Input::getInstance();
	input->SetWindowContext(GetWindow());
	input->HideCursor();

	scene = new Node("Scene");
	SetScene(scene);

	terrain = new Node("MyTerrain", scene);
	ModelImporter::getInstance()->LoadRandomTerrain(terrain, 128, 128, vec3(10.0f, 50.0f, 10.0f), TERRAIN_TEXTURE_PATH);

	heli = new Helicopter();
	helipad = new Helipad();

	Terrain* aTerrain = (Terrain*)terrain->GetComponent("Terrain");

	float spaceshipX = aTerrain->GetHeightmapRows() * 0.5f * aTerrain->GetScale().x;
	float spaceshipZ = aTerrain->GetHeightmapColumns() * 0.5f * aTerrain->GetScale().z;
	float minHelipadX = 200.0f;
	float minHelipadZ = 200.0f;
	float maxHelipadX = aTerrain->GetHeightmapRows() * aTerrain->GetScale().x - minHelipadX;
	float maxHelipadZ = aTerrain->GetHeightmapColumns() * aTerrain->GetScale().z - minHelipadZ;
	float helipadY = aTerrain->GetScale().y;

	heli->Start(scene, vec3(spaceshipX, 225.0f, spaceshipZ), 7000.0f, 5000.0f, 1000.0f, 1500.0f);
	helipad->Start(scene, helipadY, vec2(minHelipadX, minHelipadZ), vec2(maxHelipadX, maxHelipadZ));

	vec3 helipadPosition = helipad->GetPlatform()->transform->GetGlobalPosition();
	vec2 helipadGridPos(helipadPosition.z / aTerrain->GetScale().z, helipadPosition.x / aTerrain->GetScale().x);
	float flattenHeight = helipadPosition.y / aTerrain->GetScale().y * MAX_BYTE_VALUE;

	aTerrain->FlattenArea(terrain, helipadGridPos.x - 4, helipadGridPos.x + 4, helipadGridPos.y - 4, helipadGridPos.y + 4, flattenHeight);

	PhysicsManager::getInstance()->SetCurrentSceneGravity(vec3(0.0f, -1.62f, 0.0f));

	return true;
}

bool Game::OnStop()
{
	delete scene;

	return true;
}

bool Game::OnUpdate()
{
	switch (gameState)
	{
	case CONTINUE:
	{
		heli->Update();

		//float cameraSpeed = 50.0f;

		/*if (input->isInput(GLFW_KEY_W))
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
			camera->Rise(BACK);*/
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

	if (gameState == 0)
	{
		input->PollEvents();
	}

	if (input->isInput(GLFW_KEY_ESCAPE)) return false;

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