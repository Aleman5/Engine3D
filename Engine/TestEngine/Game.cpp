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
	mainCamera->Teleport(0.0f, 0.0f, -50.0f);
	mainCamera->SetAsMainCamera();

	/*nDebugCamera = new Node("DebugCameraHolder", sceneBSPTest);
	debugCamera = (Camera*)nDebugCamera->AddComponent(new Camera(true));
	debugCamera->Teleport(0.0f, 140.0f, -70.0f);
	debugCamera->Pitch(45.0f);
	debugCamera->SetSpeed(200.0f);
	debugCamera->SetAsMainDebugCamera();*/

	//scene->ActivateCameraDebugMode();

	nObjects = new Node("Objects", sceneBSPTest);

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
	
	nBSP1 = new Node("BSP1", sceneBSPTest);
	Renderer::getInstance()->AddBSP((BSP*)nBSP1->AddComponent(new BSP(vec4(0.0f, 0.0f, 0.0f, 1.0f))));

	nBSP2 = new Node("BSP1", sceneBSPTest);
	Renderer::getInstance()->AddBSP((BSP*)nBSP2->AddComponent(new BSP(vec4(0.0f, 0.0f, 0.0f, 1.0f))));

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