#include "Tilemap.h"

Tilemap::Tilemap(
	Renderer* renderer,				// Renderer reference
	Material* material,				// Material reference
	Layers layer,					// Layer of the Entity
	const char* tilesetPath,		// Path of the Tileset
	const unsigned int tColumns,	// Columns on the Tileset
	const unsigned int tRows,		// Rows on the Tileset
	const char* levelPath,			// Path of the Level
	const unsigned int levelWidth,	// Width of the Level
	const unsigned int levelHeight	// Height of the Level
) : Entity(renderer, material, layer),
tilesetColumns(tColumns), tilesetRows(tRows),
levelWidth(levelWidth), levelHeight(levelHeight),
texturePath(tilesetPath)
{
	header = TextureImporter::loadBMP_custom(texturePath);
	textureId = renderer->GenTilemapTexture(header.width, header.height, header.data);

	tileWidth = header.width / tilesetColumns;
	tileHeight = header.height / tilesetRows;

	levelColumns = (int)(levelWidth / (header.width / tColumns));
	levelRows	 = (int)(levelHeight / (header.height / tRows));

	unsigned int windowWidth = renderer->GetWindowWidht();
	unsigned int windowHeight = renderer->GetWindowHeight();

	activeTilesColumns = (windowWidth % tileWidth == 0) ? windowWidth / tileWidth + 1 : windowWidth / tileWidth + 1;
	activeTilesRows = (windowHeight % tileHeight == 0) ? windowHeight / tileHeight + 1 : windowHeight / tileHeight + 1;

	lastRowOffset = windowHeight % tileHeight;
	lastColumnOffset = windowWidth % tileWidth;

	actualPosition = realPosition = vec2(0.0f, 0.0f);

	int totalActiveTiles = activeTilesRows * activeTilesColumns;

	level = LoadLevel(levelPath);
	tiles = LoadTiles();
	uvBufferData = GenerateUvForBuffer();

	int totalTiles = activeTilesRows * activeTilesColumns;

	float vertexBufferSize = sizeof(float) * totalTiles * countOfVertices * variables;
	vertexBufferData = SetOnScreenTilesVertices();
	vertexBufferId = renderer->GenBuffer(vertexBufferData, vertexBufferSize);

	verticesData = new float[countOfVertices * 3]{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	bufferId = SetVertices(verticesData, countOfVertices);

	drawMode = GL_QUADS;
}

Tilemap::~Tilemap()
{
	delete[] verticesData;
	delete[] uvBufferData;
}

vector<vector<int>> Tilemap::LoadLevel(const char* levelPath)
{
	std::ifstream levelFile;
	
	vector<vector<int>> level(levelRows, vector<int>(levelColumns, 0));

	char buffer[BUFFER_SIZE];

	float rows = levelRows;
	float columns = levelColumns;

	levelFile.open(levelPath, std::ios::in);

	if (!levelFile.good())
		throw std::ios::failure("The level file could not be opened.");

	levelFile.getline(buffer, BUFFER_SIZE);
	levelFile.get(buffer, BUFFER_SIZE, '>');
	levelFile.get();
	memset(buffer, 0, sizeof(buffer));

	int x = 0;
	int y = 0;
	int nextChar = levelFile.peek();

	while (isdigit(nextChar) && !levelFile.eof())
	{
		if (x < levelColumns - 1)
			levelFile.get(buffer, BUFFER_SIZE, ',');
		else
		{
			if (y < levelRows - 1)
				levelFile.get(buffer, BUFFER_SIZE, '\n');
			else
				levelFile.get(buffer, BUFFER_SIZE, ',');
		}

		int digits = 0;
		int value = 0;

		for (int i = 0; buffer[i] != '\0'; i++)
			digits++;
		for (int i = 0; i < digits; i++)
			value += ((int)buffer[i] - (int)'0') * pow(10, digits - 1 - i);

		level[y][x] = value;
		
		if (x < levelColumns - 1)
			x++;
		else
		{
			x = 0;
			y++;
		}
		levelFile.get();
		nextChar = levelFile.peek();
	}

	return level;
}

vector<vector<Tile>> Tilemap::LoadTiles()
{
	vector<vector<Tile>> tiles(tilesetRows, vector<Tile>(tilesetColumns));

	for (int y = 0; y < tilesetRows; y++)
		for (int x = 0; x < tilesetColumns; x++)
		{
			tiles[y][x].index = x + y * tilesetColumns;
			tiles[y][x].type = Background;

			float minU =		(float) (x * tileWidth				) / (float) header.width;
			float maxU =		(float) (x * tileWidth + tileWidth	) / (float) header.width;
			float minV = 1.0f - (float) (y * tileHeight	+ tileHeight) / (float) header.height;
			float maxV = 1.0f - (float) (y * tileHeight				) / (float) header.height;

			float* uvData = new float[8]
			{
				minU, minV,
				minU, maxV,
				maxU, maxV,
				maxU, minV
			};

			tiles[y][x].uvData = uvData;
		}

	return tiles;
}

float* Tilemap::GenerateUvForBuffer()
{
	int totActiveTiles = activeTilesRows * activeTilesColumns;

	float* temp = new float[countOfVertices * 2 * totActiveTiles];

	float defaultUvVertices[8] =
	{
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	int counter = 0;

	for (int y = 0; y < activeTilesRows; y++)
		for (int x = 0; x < activeTilesColumns; x++)
			for (int i = 0; i < countOfVertices * 2; i++, counter++)
				temp[counter] = defaultUvVertices[i];

	return temp;
}

float* Tilemap::SetOnScreenTilesVertices()
{
	int totalTiles = activeTilesRows * activeTilesColumns;

	float* vertexBufferData = new float[countOfVertices * variables * totalTiles];

	int counter = 0;

	for (int y = 0; y < activeTilesRows; y++)
		for (int x = 0; x < activeTilesColumns; x++)
		{
			float minX = x * tileWidth;
			float maxX = x * tileWidth + tileWidth;
			float minY = (float)renderer->GetWindowHeight() - (float)(y * tileHeight + tileHeight);
			float maxY = (float)renderer->GetWindowHeight() - (float)(y * tileHeight);

			float vertices[12] =
			{
				minX, minY, 0.0f,
				minX, maxY, 0.0f,
				maxX, maxY, 0.0f,
				maxX, minY, 0.0f
			};
			
			for (int i = 0; i < countOfVertices * variables; i++, counter++)
				vertexBufferData[counter] = vertices[i];
		}

	return vertexBufferData;
}

void Tilemap::UpdateUV()
{
	int uvBufferSize = sizeof(float) * countOfVertices * 2 * activeTilesRows * activeTilesColumns;
	int counter = 0;

	glm::vec2 tilingOffset((int)renderer->GetCameraPosition().x / tileWidth, (int)renderer->GetCameraPosition().y / tileHeight);
	int lastRow = (int)levelHeight / (int)tileHeight - 1;
	int lastColumn = (int)levelWidth / (int)tileWidth - 1;

	for (int y = 0; y < activeTilesRows; y++)
		for (int x = 0; x < activeTilesColumns; x++)
		{
			int levelRow = min(y + (int)tilingOffset.y, lastRow);
			int levelColumn = min(x + (int)tilingOffset.x, lastColumn);
			
			Tile tile = GetTile(level[levelRow][levelColumn]);

			for (int i = 0; i < countOfVertices * 2; i++, counter++)
				uvBufferData[counter] = tile.uvData[i];
		}

	uvBufferId = renderer->GenBuffer(uvBufferData, uvBufferSize);
}

void Tilemap::Move(float x, float y)
{
	realPosition.x += x;
	realPosition.y += y;

	vec2 diff = realPosition - actualPosition;

	if (diff.x >= tileWidth * 2)
	{
		Translate(vec3(diff.x, 0.0f, 0.0f));

		actualPosition = realPosition;

		UpdateUV();
	}

	/*if (fabsf(diff.y) >= tileHeight * 2)
	{
		Translate(vec3(0.0f, diff.y, 0.0f));

		actualPosition = realPosition;
	}*/

	/*float screenOffsetX = renderer->GetWindowWidht();
	float translateX = 0.0f;

	if (vectorPosition.x + accumTrans.x + x < levelWidth - screenOffsetX)
		translateX = (vectorPosition.x + accumTrans.x + x > 0.0f) ? x : -(vectorPosition.x + accumTrans.x);

	accumTrans.x += translateX;

	float accumTransX = abs(accumTrans.x);

	if (accumTransX >= tileWidth)
	{
		if (accumTransX >= tileWidth)
		{
			Translate(glm::sign(accumTrans.x) * (int)tileWidth, 0.0f, 0.0f);
			accumTrans.x -= glm::sign(accumTrans.x) * (int)tileWidth;
		}
		UpdateUV();
	}*/

	/*float screenOffsetX = renderer->GetWindowWidht();
	float screenOffsetY = renderer->GetWindowHeight();
	float translateX = 0.0f;
	float translateY = 0.0f;

	if (vectorPosition.x + accumTrans.x + x < levelWidth - screenOffsetX)
		translateX = (vectorPosition.x + accumTrans.x + x > 0.0f) ? x : -(vectorPosition.x + accumTrans.x);
	else
		translateX = levelWidth - screenOffsetX - vectorPosition.x - accumTrans.x;

	if (vectorPosition.y + accumTrans.y + y < levelHeight - screenOffsetY)
		translateY = (vectorPosition.y + accumTrans.y + y > 0.0f) ? y : -(vectorPosition.y + accumTrans.y);
	else
		translateY = levelHeight - screenOffsetY - vectorPosition.y - accumTrans.y;

	accumTrans.x += translateX;
	accumTrans.y += translateY;

	float accumTransX = abs(accumTrans.x);
	float accumTransY = abs(accumTrans.y);

	if (accumTransX >= tileWidth || accumTransY >= tileHeight)
	{
		if (accumTransX >= tileWidth)
		{
			Translate(glm::sign(accumTrans.x) * (int)tileWidth, 0.0f, 0.0f);
			accumTrans.x -= glm::sign(accumTrans.x) * (int)tileWidth;
		}

		if (accumTransY >= tileHeight)
		{
			Translate(0.0f, glm::sign(accumTrans.y) * (int)tileHeight, 0.0f);
			accumTrans.y -= glm::sign(accumTrans.y) * (int)tileWidth;
		}

		UpdateUV();
	}

	//renderer->MoveCamera(vec3((vectorPosition.x + accumTrans.x) * Defs::getInstance()->deltaTime, (vectorPosition.y + accumTrans.y) * Defs::getInstance()->deltaTime, 0.0f));
	*/
}

void Tilemap::SetTileProperty(unsigned int index, TileType type)
{
	if (index >= tilesetColumns * tilesetRows)
	{
		printf("Index is out of range. The Tile tried to found doesn't exist");
		return;
	}

	unsigned int row	= index / tilesetColumns;
	unsigned int column = index % tilesetColumns;

	tiles[row][column].type = type;
}

void Tilemap::Draw()
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
	renderer->BindBuffer(vertexBufferId, 0);
	renderer->BindTextureBuffer(uvBufferId, 1);
	renderer->DrawBuffer(0, countOfVertices * activeTilesColumns * activeTilesRows, drawMode);
	renderer->DisableAttributes(0);
	renderer->DisableAttributes(1);
}

void Tilemap::ShouldDispose()
{
	if (shouldDispose)
	{
		renderer->DestroyBuffer(bufferId);
		delete[] vertexBufferData;
		shouldDispose = false;
	}
}

unsigned int Tilemap::SetVertices(float* vertices, int count)
{
	unsigned int id = renderer->GenBuffer(vertexBufferData, sizeof(float) * count * variables);
	shouldDispose = true;

	return id;
}

void Tilemap::SetOriginalPosition(float x, float y)
{
	actualPosition = realPosition = vec2(x, y);

	Translate(actualPosition.x, actualPosition.y, 0.0f);
}

Tile Tilemap::GetTile(unsigned int pos)
{
	if (pos >= tilesetRows * tilesetColumns)
	{
		printf("The position (%s) tried to found is out of range.", pos);
		return Tile();
	}
	
	unsigned int column = pos % tilesetColumns;
	unsigned int row	= pos / tilesetColumns;

	return tiles[row][column];
}

TileType Tilemap::GetTileType(unsigned int row, unsigned int column)
{
	return GetTile(level[row][column]).type;
}

glm::vec2 Tilemap::WorldToGrid(float posY, float posX)
{
	unsigned int row = (levelRows - 3) - (int)posX / tileHeight;
	unsigned int col = posY / tileWidth;

	return glm::vec2(row, col);
}

glm::vec2 Tilemap::GridToWorld(unsigned int row, unsigned int col)
{
	float posY = -((int)(row - levelRows + 1) * (int)tileHeight) + lastRowOffset;
	float posX = col * tileWidth;

	return glm::vec2(posX, posY);
}