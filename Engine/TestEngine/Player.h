#pragma once

#include "Input.h"
#include "Sprite.h"
#include "Tilemap.h"

class Player
{
	Input* input;
	Sprite* sprite;
	TileType hit;

	vec2 originalPosition;
	float speed;

public:
	void Update();
	void Draw();
	void Move();
	void ReturnToOriginalPosition();

	void Die();

	void SetSpeed(float speed) { this->speed = speed; };
	float GetSpeed() { return speed; };
	Sprite* GetSprite() { return sprite; };
	TileType GetHit() { return hit; };

	void SetOriginalPosition(float x, float y);

	Player(
		Renderer* renderer,			// Renderer reference
		Material* material,			// Material reference
		Layers layer,				// Layer of the Entity
		const char* imagePath,		// Path of the image
		bool isAnimated,			// Is animated? Yes/No
		const unsigned int sColumns,// Columns of the spritesheet
		const unsigned int sRows,	// Rows of the spritesheet
		const float colliderWidth,	// Width of the collider
		const float colliderHeight,	// Height of the collider
		bool isInteractable,		// Does the Sprite collide with the Tilemap? Yes/No
		Tilemap* tilemap			// Tilemap reference
	);
	~Player();
};

