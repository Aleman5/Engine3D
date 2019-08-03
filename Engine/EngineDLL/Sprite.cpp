#include "Sprite.h"

Sprite::Sprite(
	Renderer* renderer,			 // Renderer reference
	Material* material,			 // Material reference
	Layers layer,				 // Layer of the Entity
	const char* imagePath,		 // Path of the image
	bool isAnimated,			 // Is animated? Yes/No
	const unsigned int sColumns, // Columns of the spritesheet
	const unsigned int sRows, 	 // Rows of the spritesheet
	const float colliderWidth, 	 // Width of the collider
	const float colliderHeight,	 // Height of the collider
	bool isInteractable,		 // Does the Sprite collide with the Tilemap? Yes/No
	Tilemap* tilemap			 // Tilemap reference
)
	: Shape(renderer, material, layer), actualFrame(0), columns(sColumns), rows(sRows),
	  isAnimated(isAnimated), isInteractable(isInteractable), tilemap(tilemap),
	  texturePath(imagePath)
{
	count = 4;
	variables = 3;

	header = TextureImporter::loadBMP_custom(imagePath);

	widthOfFrame = (int)(header.width / sColumns);
	heightOfFrame = (int)(header.height / sRows);

	float valueX = (float)widthOfFrame / 2;
	float valueY = (float)heightOfFrame / 2;

	float* verticesData = new float[count * variables]
	{
		-valueX, valueY, 0.0f,
		valueX, valueY, 0.0f,
		-valueX, -valueY, 0.0f,
		valueX, -valueY, 0.0f
	};

	drawMode = GL_TRIANGLE_STRIP;

	bufferId = SetVertices(verticesData, count);
	textureId = renderer->GenTexture(header.width, header.height, header.imageFormat, header.data);

	unsigned int frames[1] = { 0 };
	if (isAnimated) anim = new Animation(this, frames, true, 10.0f);
	else SetNewFrame(0);

	col.x = colliderWidth;
	col.y = colliderHeight;
}

Sprite::~Sprite()
{
	delete[] verticesData;
	if (isAnimated) delete anim;
}

void Sprite::Draw()
{
	renderer->loadIdentityMatrix();
	renderer->SetModelMatrix(model);

	if (material != NULL)
	{
		material->Bind(texturePath, textureId);
		material->SetMatrixProperty("MVP", renderer->GetMVP());
	}

	renderer->EnableAttributes(0);
	renderer->EnableAttributes(1);
	renderer->BindBuffer(bufferId, 0);
	renderer->BindTextureBuffer(uvBufferId, 1);
	renderer->DrawBuffer(count, drawMode);
	renderer->DisableAttributes(0);
	renderer->DisableAttributes(1);
}

void Sprite::Update()
{
	if (isAnimated) anim->Update();

	Shape::Update();
}

TileType Sprite::Move(float x, float y, float z)
{
	Translate(x, y, z);

	TileType tileType = Background;

	if (isInteractable)
		tileType = CheckTileTypeCollisionWith(x, y, z);

	return tileType;
}

unsigned int Sprite::SetTextureUV(float* vertices, int count, int variables)
{
	unsigned int id = renderer->GenBuffer(vertices, sizeof(float) * count * variables);
	shouldDispose = true;

	return id;
}

void Sprite::SetNewFrame(unsigned int newFrame)
{
	actualFrame = newFrame;
	
	unsigned int uvBufferSize = sizeof(float) * count * 2;

	unsigned int u =	  (actualFrame % rows	) * widthOfFrame;
	unsigned int v = (int)(actualFrame / columns) * heightOfFrame;
	
	uvBufferData = SetUV(u, v);

	uvBufferId = renderer->GenBuffer(uvBufferData, uvBufferSize);
}

float* Sprite::SetUV(unsigned int u, unsigned int v)
{
	float minU =		(float)  u					/ header.width;
	float maxU =		(float) (u + widthOfFrame)	/ header.width;
	float minV = 1.0f - (float)  v					/ header.height;
	float maxV = 1.0f - (float) (v + heightOfFrame) / header.height;

	float* bufferData = new float[count * 2]
	{
		minU, minV,
		maxU, minV,
		minU, maxV,
		maxU, maxV,
	};

	return bufferData;
}

void Sprite::SetOriginalPosition(float x, float y)
{
	originalPosition.x = x;
	originalPosition.y = y;

	Teleport(originalPosition.x, originalPosition.y, 0.0f);
}

TileType Sprite::CheckTileTypeCollisionWith(float x, float y, float z)
{
	TileType tileType = Background;

	float horOffset = col.x / 2.0f;
	float verOffset = col.y / 2.0f;
	float newPosX = GetPosition().x;
	float newPosY = GetPosition().y;
	int possibleHorCols = glm::max(col.y / heightOfFrame + 1.0f, 2.0f);
	int possibleVerCols = glm::max(col.x / widthOfFrame + 1.0f, 2.0f);

	if (x != 0.0f)
	{
		for (int i = -possibleHorCols / 2; i <= possibleHorCols / 2; i++)
		{
			if (x > 0.0f)
			{
				glm::vec2 rightTileCoord = tilemap->WorldToGrid(
					GetPosition().x + horOffset,
					GetPosition().y + (verOffset / (possibleHorCols / 2)) * i + heightOfFrame);

				tileType = tilemap->GetTileType(
					rightTileCoord.x,
					rightTileCoord.y);

				if (tileType == Obstacle)
				{
					newPosX = (tilemap->GridToWorld(
						rightTileCoord.x,
						rightTileCoord.y)
						).x - horOffset;
					break;
				}
			}
			else
			{
				glm::vec2 leftTileCoord = tilemap->WorldToGrid(
					GetPosition().x - horOffset,
					GetPosition().y + (verOffset / (possibleHorCols / 2)) * i + heightOfFrame);

				tileType = tilemap->GetTileType(
					leftTileCoord.x,
					leftTileCoord.y);

				if (tileType == Obstacle)
				{
					newPosX = (tilemap->GridToWorld(leftTileCoord.x,
						leftTileCoord.y)
						).x + horOffset + widthOfFrame;
					break;
				}
			}
		}
	}

	if (y != 0.0f)
	{
		for (int i = -possibleVerCols / 2; i <= possibleVerCols / 2; i++)
		{
			if (y > 0.0f)
			{
				glm::vec2 upperTileCoord = tilemap->WorldToGrid(
					GetPosition().x + (horOffset / (possibleVerCols / 2)) * i,
					GetPosition().y + verOffset);

				tileType = tilemap->GetTileType(
					upperTileCoord.x,
					upperTileCoord.y);

				if (tileType == Obstacle)
				{
					newPosY = (tilemap->GridToWorld(
						upperTileCoord.x,
						upperTileCoord.y)
						).y - verOffset - tilemap->GetLastRowOffset();
					break;
				}
			}
			else
			{
				glm::vec2 lowerTileCoord = tilemap->WorldToGrid(
					GetPosition().x + (horOffset / (possibleVerCols / 2)) * i,
					GetPosition().y - verOffset);

				tileType = tilemap->GetTileType(
					lowerTileCoord.x,
					lowerTileCoord.y);
				
				if (tileType == Obstacle)
				{
					newPosY = (tilemap->GridToWorld(
						lowerTileCoord.x,
						lowerTileCoord.y)
						).y + verOffset;
					break;
				}
			}
		}
	}

	if (newPosX != GetPosition().x || newPosY != GetPosition().y)
		Teleport(newPosX, newPosY, GetPosition().z);

	return tileType;
}