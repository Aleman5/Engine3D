#pragma once

#include "Exports.h"
#include "Definitions.h"
#include "Entity.h"
#include "TextureImporter.h"

#define BUFFER_SIZE 300

enum TileType
{
	Obstacle,
	Background,
	Win_Trigger,
	Death_Trigger
};

struct Tile
{
	int index;
	TileType type;
	float* uvData;
};

struct TilesAround
{
	glm::vec3 pos[4];
	TileType type[4];
};

class ENGINEDLL_API Tilemap : public Entity
{
	Header header;						// Information of the Texture

	float* verticesData;				// Data of the Vertices

	unsigned int uvBufferId;			// Id of the UvBuffer
	unsigned int vertexBufferId;		// Id of the VertexBuffer
	unsigned int textureId;				// Id of the Texture
	float* uvBufferData;				// Data of the UvBuffer
	float* vertexBufferData;			// Data of the VertexBuffer

	vector<vector<int>> level;			// Info of each Tile of the Level
	vector<vector<Tile>> tiles;			// Info of each Tile of the Tileset

	const unsigned int countOfVertices = 4;
	const unsigned int variables = 3;

	unsigned int levelColumns;			// Amount of columns on the Level
	unsigned int levelRows;				// Amount of rows on the Level
	unsigned int levelWidth;			// Width of the Level
	unsigned int levelHeight;			// Height of the Level

	unsigned int tilesetColumns;		// Amount of columns on the Tileset
	unsigned int tilesetRows;			// Amount of rows on the Tileset
	unsigned int tileWidth;				// Width of each Tileset
	unsigned int tileHeight;			// Height of each Tileset

	unsigned int activeTilesColumns;	// Amount of columns that will be showed
	unsigned int activeTilesRows;		// Amount of Rows that will be showed

	float lastRowOffset;
	float lastColumnOffset;

	float speed;

	vec2 actualPosition;
	vec2 realPosition;

	vec2 accumTrans;

	const char* texturePath;

	vector<vector<int>> LoadLevel(
		const char* levelPath			// Path of the Level
	);

	float* GenerateUvForBuffer();
	float* SetOnScreenTilesVertices();

	vector<vector<Tile>> LoadTiles();

public:
	void Draw() override;
	void Move(float x, float y);
	void UpdateUV();
	void SetTileProperty(
		unsigned int index,	// Index of the Tile
		TileType type		// TileType of the specific Tileset
	);

	void ShouldDispose() override;

	unsigned int SetVertices(
		float* vertices,	// Data of the vertices
		int count			// Total Vertices
	) override;

	void SetSpeed(float speed) { this->speed = speed; };
	void SetOriginalPosition(float x, float y);

	Tile GetTile(
		unsigned int pos	// Position of the Tile
	);

	TileType GetTileType(unsigned int row, unsigned int column);

	glm::vec2 WorldToGrid(float posY, float posX);
	glm::vec2 GridToWorld(unsigned int row, unsigned int col);

	float GetSpeed() { return speed; };
	float GetLastRowOffset() const { return lastRowOffset; };
	float GetLastColumnOffset() const { return lastColumnOffset; };

	Tilemap(
		Renderer* renderer,				// Renderer reference
		Material* material,				// Material reference
		Layers layer,					// Layer of the Entity
		const char* tilesetPath,		// Path of the Tileset
		const unsigned int tColumns,	// Columns on the Tileset
		const unsigned int tRows,		// Rows on the Tileset
		const char* levelPath,			// Path of the Level
		const unsigned int levelWidth,	// Width of the Level
		const unsigned int levelHeight	// Height of the Level
	);
	~Tilemap();
};