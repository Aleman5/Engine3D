#pragma once

// Important Includes
#include "GameBase.h"
#include "Node.h"
#include "Input.h"
#include "CollisionManager.h"

// Included in Node.h
#include "Camera.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Tilemap.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Circle.h"

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
	Camera* camera;
	Input* input;
	Material* material;
	Material* matTexture;
	Material* matMesh;

	Mesh* M4A1;
	Mesh* spider;
	Mesh* thorHammer;
#pragma endregion

#pragma region Version with Scene Graph
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

