#include "Game.h"
#include "GlobalDefs.h"
#include "BSP.h"

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

	simEventCb = new SimulationEventCallback();

	// ------------------------- Start ------------------------- //
	// -------------------- Scene BSP Test --------------------- //
	// --------------------------------------------------------- //

	speed = 2.0f;
	gameState = CONTINUE;

	sceneBSPTest = new Node("Scene");
	SetScene(sceneBSPTest);

	nCamera = new Node("CameraHolder", sceneBSPTest);
	mainCamera = (Camera*)nCamera->AddComponent(new Camera(true));
	mainCamera->Teleport(0.0f, 0.0f, -30.0f);
	mainCamera->SetAsMainCamera();

	/*nDebugCamera = new Node("DebugCameraHolder", sceneBSPTest);
	debugCamera = (Camera*)nDebugCamera->AddComponent(new Camera(true));
	debugCamera->Teleport(0.0f, 140.0f, -70.0f);
	debugCamera->Pitch(45.0f);
	debugCamera->SetSpeed(200.0f);
	debugCamera->SetAsMainDebugCamera();*/

	//scene->ActivateCameraDebugMode();

	nObjects = new Node("Objects", sceneBSPTest);

	/*nWeapon = new Node("M4A1", nObjects);
	nWeapon->transform->SetLayer(Character);
	nWeapon->transform->Teleport(0.0f, 0.0f, 0.0f);

	nWeaponMesh = new Node("M4A1Mesh", nWeapon);
	nWeaponMesh->transform->Scale(0.1f, 0.1f, 0.1f);
	ModelImporter::getInstance()->Load(nWeaponMesh, M4A1_PATH, M4A1_TEXTURE_PATH);
	//nWeaponMesh->ActivateMeshDebugMode();

	nSpider = new Node("Spider", nWeapon);
	nSpider->transform->SetLayer(Enemy);
	nSpider->transform->Teleport(-15.0f, 0.0f, 15.0f);

	nSpiderMesh = new Node("SpiderMesh", nSpider);
	nSpiderMesh->transform->Scale(0.15f, 0.15f, 0.15f);
	ModelImporter::getInstance()->Load(nSpiderMesh, SPIDER_PATH, SPIDER_TEXTURE_PATH);
	//nSpiderMesh->ActivateMeshDebugMode();*/

	/*nHelicopter = new Node("Helicopter", nObjects);
	nHelicopter->transform->SetLayer(Character);
	nHelicopter->transform->Teleport(18.0f, 0.0f, -12.0f);
	
	nHelicopterMesh = new Node("HelicopterMesh", nHelicopter);
	nHelicopterMesh->transform->Scale(0.005f, 0.005f, 0.005f);
	ModelImporter::getInstance()->Load(nHelicopterMesh, HELICOPTER_PATH, HELICOPTER_TEXTURE_PATH);*/
	//nHelicopterMesh->ActivateMeshDebugMode();
	
	nWeap1 = new Node("nWeap1", nObjects);
	nWeap1->transform->SetLayer(Character);
	nWeap1->transform->Teleport(12.0f, 0.0f, 0.0f);

	nWeap1Mesh = new Node("LaWeapon1", nWeap1);
	nWeap1Mesh->transform->Scale(0.1f, 0.1f, 0.1f);
	ModelImporter::getInstance()->Load(nWeap1Mesh, M4A1_PATH, M4A1_TEXTURE_PATH);

	nWeap2 = new Node("nWeap2", nObjects);
	nWeap2->transform->SetLayer(Character);
	nWeap2->transform->Teleport(-12.0f, 0.0f, 0.0f);

	nWeap2Mesh = new Node("LaWeapon2", nWeap2);
	nWeap2Mesh->transform->Scale(0.1f, 0.1f, 0.1f);
	ModelImporter::getInstance()->Load(nWeap2Mesh, M4A1_PATH, M4A1_TEXTURE_PATH);

	nWeap3 = new Node("nWeap3", nObjects);
	nWeap3->transform->SetLayer(Character);
	nWeap3->transform->Teleport(0.0f, 0.0f, 0.0f);
	
	nWeap3Mesh = new Node("LaWeapon3", nWeap3);
	nWeap3Mesh->transform->Scale(0.1f, 0.1f, 0.1f);
	ModelImporter::getInstance()->Load(nWeap3Mesh, M4A1_PATH, M4A1_TEXTURE_PATH);

	nWeap4 = new Node("nWeap4", nObjects);
	nWeap4->transform->SetLayer(Character);
	nWeap4->transform->Teleport(0.0f, 0.0f, 12.0f);
	
	nWeap4Mesh = new Node("LaWeapon4", nWeap4);
	nWeap4Mesh->transform->Scale(0.1f, 0.1f, 0.1f);
	ModelImporter::getInstance()->Load(nWeap4Mesh, M4A1_PATH, M4A1_TEXTURE_PATH);

	nWeap5 = new Node("nWeap5", nObjects);
	nWeap5->transform->SetLayer(Character);
	nWeap5->transform->Teleport(-16.0f, 0.0f, -6.0f);
	
	nWeap5Mesh = new Node("LaWeapon5", nWeap5);
	nWeap5Mesh->transform->Scale(0.1f, 0.1f, 0.1f);
	ModelImporter::getInstance()->Load(nWeap5Mesh, M4A1_PATH, M4A1_TEXTURE_PATH);

	timeToStart = 15.0f;
	timeLeft = timeToChange = 7.0f;
	dir = 1;

	nBSP1 = new Node("BSP1", sceneBSPTest);
	nBSP1->transform->Teleport(6.0f, 0.0f, 0.0f);
	nBSP1->transform->Rotate(0.0f, 90.0f, 0.0f);
	nBSP1->AddComponent(new BSP());

	nBSP2 = new Node("BSP2", sceneBSPTest);
	nBSP2->transform->Teleport(-6.0f, 0.0f, 0.0f);
	nBSP2->transform->Rotate(0.0f, 90.0f, 0.0f);
	nBSP2->AddComponent(new BSP());

	nBSP3 = new Node("BSP3", sceneBSPTest);
	nBSP3->transform->Teleport(0.0f, 0.0f, 6.0f);
	nBSP3->transform->Rotate(0.0f, 180.0f, 0.0f);
	nBSP3->AddComponent(new BSP());

	// --------------------------------------------------------- //
	// -------------------- Scene BSP Test --------------------- //
	// -------------------------- End -------------------------- //


	// ------------------------- Start ------------------------- //
	// -------------------- Scene Spaceship -------------------- //
	// --------------------------------------------------------- //

	
	/*sceneSpaceship = new Node("SceneSpaceship");
	SetScene(sceneSpaceship);

	terrain = new Node("MyTerrain", sceneSpaceship);
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

	heli->Start(sceneSpaceship, vec3(spaceshipX, 225.0f, spaceshipZ), 10000.0f, 10000.0f, 1000.0f, 2500.0f);
	helipad->Start(sceneSpaceship, helipadY, vec2(minHelipadX, minHelipadZ), vec2(maxHelipadX, maxHelipadZ));

	vec3 helipadPosition = helipad->GetPlatform()->transform->GetGlobalPosition();
	vec2 helipadGridPos(helipadPosition.z / aTerrain->GetScale().z, helipadPosition.x / aTerrain->GetScale().x);
	float flattenHeight = helipadPosition.y / aTerrain->GetScale().y * MAX_BYTE_VALUE;

	aTerrain->FlattenArea(terrain, helipadGridPos.x - 4, helipadGridPos.x + 4, helipadGridPos.y - 4, helipadGridPos.y + 4, flattenHeight);

	simEventCb->AddSpaceshipRigidActor(heli->GetRigidActor());
	simEventCb->AddHelipadRigidActor(helipad->GetRigidActor());*/

	// --------------------------------------------------------- //
	// -------------------- Scene Spaceship -------------------- //
	// -------------------------- End -------------------------- //

	PhysicsManager::getInstance()->SetSimulationEventCallback(simEventCb);
	PhysicsManager::getInstance()->SetCurrentSceneGravity(vec3(0.0f, -1.5f, 0.0f));

	return true;
}

bool Game::OnStop()
{
	delete sceneBSPTest;
	delete sceneSpaceship;
	delete simEventCb;

	return true;
}

bool Game::OnUpdate()
{
	switch (gameState)
	{
	case CONTINUE:
	{
		//heli->Update();

		//float cameraSpeed = 50.0f;

		if (input->isInput(GLFW_KEY_W))
			mainCamera->WalkFront(ADVANCE);
		if (input->isInput(GLFW_KEY_S))
			mainCamera->WalkFront(BACK);
		if (input->isInput(GLFW_KEY_A))
			mainCamera->WalkSideWays(ADVANCE);
		if (input->isInput(GLFW_KEY_D))
			mainCamera->WalkSideWays(BACK);
		if (input->isInput(GLFW_KEY_Q))
			mainCamera->Rise(ADVANCE);
		if (input->isInput(GLFW_KEY_E))
			mainCamera->Rise(BACK);
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

	timeToStart -= Defs::getInstance()->deltaTime;

	if (timeToStart <= 0.0f)
	{
		nWeap5->transform->Translate(6.0f * dir * Defs::getInstance()->deltaTime, 0.0f, 0.0f);

		timeLeft -= Defs::getInstance()->deltaTime;

		if (timeLeft <= 0)
		{
			timeLeft = timeToChange;
			dir *= -1;
		}

		if (gameState == 0)
		{
			input->PollEvents();
		}
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