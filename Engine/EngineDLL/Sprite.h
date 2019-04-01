#pragma once

#include "Definitions.h"
#include "Shape.h"
#include "Animation.h"
#include "TextureImporter.h"
#include "Tilemap.h"

class ENGINEDLL_API Sprite : public Shape
{
	Header header;				// Information of the Texture
	Animation* anim;			// Animation reference
	Tilemap* tilemap;			// Reference to the Tilemap
	unsigned int textureId;		// Id of the Texture
	unsigned int uvBufferId;	// Id of the UvBuffer
	unsigned int actualFrame;	// Actual Frame
	unsigned int columns;		// Columns of the Spritesheet
	unsigned int rows;			// Rows of the Spritesheet
	unsigned int widthOfFrame;	// Widht of each frame
	unsigned int heightOfFrame;	// Height of each frame

	vec2 originalPosition;		// Position where the sprites restart

	float* verticesTextureData;	// Data of each vertice of the Texture (vec2)
	float* uvBufferData;		// Data of each UV of the Buffer (vec2)

	const char* texturePath;

	bool isAnimated;			// Is animated? Yes/No
	bool isInteractable;		// Does the Sprite collide with the Tilemap? Yes/No

	

public:
	void Draw() override;
	void Update();
	TileType Move(float x, float y, float z);

	TileType CheckTileTypeCollisionWith(float x, float y, float z);

	unsigned int SetTextureUV(
		float* vertices,	// Data of the vertices
		int count,			// Total Vertices
		int variables		// Total variables
	);

	// Used if the sprite is not animated
	void SetNewFrame(
		unsigned int newFrame // The next frame to render
	);

	float* SetUV(
		unsigned int x, // Equivalent to U
		unsigned int y  // Equivalent to V
	);

	void SetOriginalPosition(float x, float y);
	vec2 GetOriginalPosition() { return originalPosition; };
	Animation* GetAnimation() { if (isAnimated) return anim; };

	Sprite(Renderer* renderer,			// Renderer reference
		   Material* material,			// Material reference
		   Layers layer,				// Layer of the Entity
		   const char* imagePath,		// Path of the image
		   bool isAnimated,				// Is animated? Yes/No
		   const unsigned int sColumns, // Columns of the spritesheet
		   const unsigned int sRows,	// Rows of the spritesheet
		   const float colliderWidth,	// Width of the collider
		   const float colliderHeight,	// Height of the collider
		   bool isInteractable,			// Does the Sprite collide with the Tilemap? Yes/No
		   Tilemap* tilemap				// Tilemap reference
	);	
	~Sprite();
};