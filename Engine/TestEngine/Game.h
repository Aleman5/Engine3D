#pragma once

// Important Includes
#include "GameBase.h"
#include "Node.h"
#include "Input.h"
#include "CollisionManager.h"

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
#pragma region Version without Scene Graph
	Input* input;
	Material* material;
	Material* matTexture;
	Material* matMesh;

#pragma endregion

#pragma region Version with Scene Graph
	Node* scene;
	Camera* camera;

	Node* nCamera;
	Node* nWeapon;
	Node* nSpider;
	Node* nThorHammer;
#pragma endregion

	float speed;

	GameState gameState;

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

