#pragma once

// Important includes
#include "GameBase.h"
#include "Node.h"
#include "Input.h"
#include "CollisionManager.h"

// Common includes
#include "Camera.h"
#include "Mesh.h"
#include "Box.h"

// Extra includes
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

	Node* scene;
	Node* nCamera;
	Node* nWeapon;
	Node* nSpider;
	Node* nThorHammer;

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

