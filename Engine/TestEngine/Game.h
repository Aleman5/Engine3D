#pragma once

#include "GameBase.h"
#include "Camera.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Sprite.h"
#include "Tilemap.h"
#include "Input.h"
#include "CollisionManager.h"
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
	Camera* camera;
	Input* input;
	Material* material;
	Material* matTexture;
	Player* player;
	list<Asteroid*>* asteroids;
	Tilemap* tilemap;

	float speed;

	const int totalAsteroids = 25;

	GameState gameState;

	void FillAsteroidsData();
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

