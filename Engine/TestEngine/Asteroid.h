#pragma once

#include "Input.h"
#include "Sprite.h"

class Asteroid
{
	Input* input;
	Sprite* sprite;

	vec2 originalPosition;
	float speed;

public:
	void Update();
	void Draw();
	void Move();
	void ReturnToOriginalPosition();

	void SetSpeed(float speed) { this->speed = speed; };
	float GetSpeed() { return speed; };
	Sprite* GetSprite() { return sprite; };

	void SetOriginalPosition(float x, float y);

	Asteroid(
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
	~Asteroid();
};

