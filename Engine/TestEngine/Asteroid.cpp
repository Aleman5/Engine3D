#include "Asteroid.h"

Asteroid::Asteroid(
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
)
{
	input = Input::getInstance();

	sprite = new Sprite(renderer, material, layer, imagePath, isAnimated, sColumns, sRows, colliderWidth, colliderHeight, isInteractable, tilemap);
	
}
Asteroid::~Asteroid()
{
}

void Asteroid::Update()
{
	sprite->Update();
}

void Asteroid::Draw()
{
	sprite->Draw();
}

void Asteroid::Move()
{
	sprite->Translate(-speed * Defs::getInstance()->deltaTime / 2, 0.0f, 0.0f);
}

void Asteroid::SetOriginalPosition(float x, float y)
{
	originalPosition.x = x;
	originalPosition.y = y;

	sprite->Teleport(originalPosition.x, originalPosition.y, 0.0f);
}

void Asteroid::ReturnToOriginalPosition()
{
	sprite->Teleport(
		originalPosition.x,
		originalPosition.y,
		0.0f);
}