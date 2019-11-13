#pragma once

// Engine includes
#include "GameBase.h"
#include "GlobalDefs.h"
#include "ModelImporter.h"
#include "Node.h"
#include "Input.h"
#include "CollisionManager.h"
#include "SimulationEventCallback.h"
#include "Terrain.h"
#include "Camera.h"
#include "Mesh.h"
#include "Box.h"

// Extra includes
#include "GamePaths.h"
#include "Helicopter.h"
#include "Helipad.h"

enum GameState
{
	LOSE = -1,
	CONTINUE,
	WIN
};

class Game : public GameBase
{
	Input* input;
	GameState gameState;
	SimulationEventCallback* simEventCb;

	Camera* mainCamera;

	Node* nCamera;
	Node* nDebugCamera;
	Node* nObjects;
	Node* nWeapon;
	Node* nWeaponMesh;
	Node* nSpider;
	Node* nSpiderMesh;
	Node* nHelicopter;
	Node* nHelicopterMesh;

	Node* nWeap1;
	Node* nWeap1Mesh;
	Node* nWeap2;
	Node* nWeap2Mesh;
	Node* nWeap3;
	Node* nWeap3Mesh;
	Node* nWeap4;
	Node* nWeap4Mesh;
	Node* nWeap5;
	Node* nWeap5Mesh;

	Node* nBSP1;
	Node* nBSP2;
	Node* nBSP3;

	Node* sceneBSPTest;
	Node* sceneSpaceship;
	Node* terrain;
	Helicopter* heli;
	Helipad* helipad;

	float speed;
	float timeToStart;
	float timeToChange;
	float timeLeft;
	float dir;

	void Restart();

protected:
	bool OnStart() override;
	bool OnStop() override;
	bool OnUpdate() override;
	bool OnDraw() override;

public:
	Game();
	~Game();
};

