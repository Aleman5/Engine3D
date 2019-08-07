#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "RandomHeightGenerator.h"
#include "GlobalDefs.h"
#include "ModelImporter.h"
#include "Terrain.h"
#include "Node.h"
#include "Mesh.h"


ModelImporter * ModelImporter::instance = NULL;

ModelImporter::ModelImporter()
{
}

void ModelImporter::Load(Node* thisNode, const string modelPath, const string texturePath)
{
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(modelPath.c_str(), ASSIMP_LOAD_FLAGS_TRIANG_FLIP);

	if (!pScene)
		printf("Error parsing '%s': '%s'\n", modelPath.c_str(), Importer.GetErrorString());

	AttendNode(pScene, pScene->mRootNode, thisNode, thisNode->fcData, modelPath, texturePath);

	thisNode->fcData.UpdateData();
}

void ModelImporter::LoadTerrain(Node* thisNode, const string& heightmapPath, vec3 scale,
	const string& texturesPath)
{
	//Node* terrainObject = new Node("Terrain", parent);

	int hmColumns, hmRows;
	unsigned char* heightmap = Header::LoadHeightmap(heightmapPath, hmColumns, hmRows);

	unsigned char* pixel = heightmap;

	vector<Vertex> vertices;
	vector<vector<int>> heights;

	vector<int> currentRowHeights;

	for (unsigned int row = 0; row < (unsigned int)hmRows; row++)
	{
		currentRowHeights.clear();

		for (unsigned int col = 0; col < (unsigned int)hmColumns; col++)
		{
			float posX = col * scale.x;
			float posY = (float)*pixel / MAX_BYTE_VALUE * scale.y;
			float posZ = row * scale.z;

			float u = (float)col / (float)hmColumns;
			float v = 1.0f - (float)row / (float)hmRows;

			Vertex vertex(vec3(posX, posY, posZ),
						  vec2(u, v),
						  vec3(0.0f, 0.0f, 0.0f));

			currentRowHeights.push_back((int)*pixel);
			vertices.push_back(vertex);
			pixel++;
		}

		heights.push_back(currentRowHeights);
	}

	vector<unsigned int> indices = GenerateTerrainIndices(hmRows, hmColumns);

	//return terrainObject;
}

void ModelImporter::LoadRandomTerrain(Node* thisNode, int rows, int columns, vec3 scale,
	const char* texturesPath)
{
	Mesh* mesh = new Mesh();

	RandomHeightGenerator::GenerateSeed();

	vector<Vertex> vertices;
	vector<vector<int>> heights;

	vector<int> currentRowHeights;

	for (unsigned int row = 0; row < (unsigned int)rows; row++)
	{
		currentRowHeights.clear();

		for (unsigned int col = 0; col < (unsigned int)columns; col++)
		{
			int randomHeight = RandomHeightGenerator::GenerateHeight(row, col);

			float posX = col * scale.x;
			float posY = (float)randomHeight / MAX_BYTE_VALUE * scale.y;
			float posZ = row * scale.z;

			float u = (float)col / (float)columns;
			float v = 1.0f - (float)row / (float)rows;

			Vertex vertex(vec3(posX, posY, posZ),
						  vec2(u, v),
						  vec3(0.0f, 0.0f, 0.0f));

			currentRowHeights.push_back(randomHeight);
			vertices.push_back(vertex);
		}

		heights.push_back(currentRowHeights);
	}

	vector<unsigned int> indices = GenerateTerrainIndices(rows, columns);

	if (texturesPath != "")
	{
		mesh->m_Textures.push_back(TextureImporter::LoadImage(texturesPath));

		unsigned int textureId = Renderer::getInstance()->GenTexture(mesh->m_Textures[0].width, mesh->m_Textures[0].height, mesh->m_Textures[0].imageFormat, mesh->m_Textures[0].data);
		mesh->m_Textures[0].id = textureId;
		mesh->bufferTextures.push_back(textureId);
	}

	
	Terrain* terrain = (Terrain*)thisNode->AddComponent(new Terrain());
	terrain->CreateHeightField(heights, rows, columns, scale);

	mesh->m_Entries.resize(1);
	mesh->m_Entries[0].Init(vertices, indices);

	mesh->SetDiffTex(mesh->m_Textures);

	thisNode->AddComponent(mesh);
}

bool ModelImporter::AttendNode(const aiScene* aiScene, aiNode* aiNode, Node* parent,
							   FCCubeData& fcData, const string modelPath, const string texturePath)
{
	for (int i = 0; i < (int)aiNode->mNumMeshes; i++)
	{
		const aiMesh* aiMesh = aiScene->mMeshes[aiNode->mMeshes[i]];

		Node* child = new Node(aiNode->mName.C_Str(), parent);
		child->AddComponent(InitMesh(aiScene, aiMesh, parent, fcData, modelPath, texturePath, i));
	}

	for (int i = 0; i < (int)aiNode->mNumChildren; i++)
	{
		AttendNode(aiScene, aiNode->mChildren[i], parent, fcData, modelPath, texturePath);
	}

	return true;
}

Mesh* ModelImporter::InitMesh(const aiScene* aiScene, const aiMesh* aiMesh, Node* parent,
							 FCCubeData& fcData, const string modelPath, const string texturePath, unsigned int Index)
{
	Mesh* mesh = new Mesh();

	mesh->m_Entries.resize(aiScene->mNumMeshes);
	mesh->m_Textures.resize(aiScene->mNumMaterials);

	mesh->m_Entries[Index].MaterialIndex = aiMesh->mMaterialIndex;

	vector<Vertex> Vertices;
	vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	float minX = 999999.0f, minY = 999999.0f, minZ = 999999.0f;
	float maxX = -999999.0f, maxY = -999999.0f, maxZ = -999999.0f;

	for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(aiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(aiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = aiMesh->HasTextureCoords(0) ? &(aiMesh->mTextureCoords[0][i]) : &Zero3D;

		Vertex v(vec3((float)pPos->x, (float)pPos->y, (float)pPos->z),
			vec2((float)pTexCoord->x, (float)pTexCoord->y),
			vec3((float)pNormal->x, (float)pNormal->y, (float)pNormal->z));

		fcData.NewValue(v.m_pos);

		Vertices.push_back(v);
	}

	for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
	{
		const aiFace& Face = aiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	mesh->m_Entries[Index].Init(Vertices, Indices);

	for (unsigned int i = 0; i < aiScene->mNumMaterials; i++)
	{
		mesh->m_Textures[i] = TextureImporter::LoadImage(texturePath.c_str());

		unsigned int textureId = Renderer::getInstance()->GenTexture(mesh->m_Textures[i].width, mesh->m_Textures[i].height, mesh->m_Textures[i].imageFormat, mesh->m_Textures[i].data);
		mesh->m_Textures[i].id = textureId;
		mesh->bufferTextures.push_back(textureId);
	}

	mesh->SetDiffTex(mesh->m_Textures);

	return mesh;
}

void ModelImporter::InitTexture()
{
	//for (int i = 0; i < m_Textures.size(); i++)
	//	bufferTextures.push_back(renderer->GenTexture(m_Textures[i].width, m_Textures[i].height, m_Textures[i].imageFormat, m_Textures[i].data));
}

vector<unsigned int> ModelImporter::GenerateTerrainIndices(int rows, int columns)
{
	vector<unsigned int> indices;

	unsigned int start = 0;
	unsigned int gridRows = rows - 1;
	unsigned int gridColumns = columns - 1;

	for (unsigned int row = 0; row < gridRows; row++)
	{
		for (unsigned int col = 0; col < gridColumns; col++)
		{
			start = row * columns + col;

			indices.push_back(start);
			indices.push_back(start + 1);
			indices.push_back(start + columns);

			indices.push_back(start + columns);
			indices.push_back(start + columns + 1);
			indices.push_back(start + 1);
		}
	}

	return indices;
}