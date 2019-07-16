#pragma once

// Engine includes
#include "GameBase.h"
#include "GlobalDefs.h"
#include "ModelImporter.h"
#include "Node.h"
#include "Input.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "Mesh.h"
#include "Box.h"

// Extra includes
#include "GamePaths.h"
#include "Player.h"
#include "Asteroid.h"

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
	Material* material;
	Material* matTexture;
	Material* matMesh;

	Camera* camera;
	Camera* debugCamera;

	Node* scene;
	Node* nCamera;
	Node* nDebugCamera;
	Node* nObjects;
	Node* nWeapon;
	Node* nWeaponMesh;
	Node* nSpider;
	Node* nSpiderMesh;
	Node* nHelicopter;
	Node* nHelicopterMesh;

	float speed;

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

