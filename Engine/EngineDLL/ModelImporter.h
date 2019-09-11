#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include "TextureImporter.h"
#include "Exports.h"
#include "Structs.h"

using namespace std;

class Mesh;
class Node;
class aiScene;
class aiNode;
class aiMesh;

class ENGINEDLL_API ModelImporter
{
	static ModelImporter *instance;

	bool AttendNode(const aiScene* aiScene, aiNode* aiNode, Node* parent,
					const string modelPath, const string texturePath);
	Mesh* InitMesh(const aiScene* aiScene, const aiMesh* aiMesh, Node* parent,
					Node* child, const string modelPath, const string texturePath, unsigned int Index);
	void InitTexture();

	static vector<unsigned int> GenerateTerrainIndices(int rows, int columns);

public:
	void Load(Node* thisNode, const string modelPath, const string texturePath);
	void LoadTerrain(Node* thisNode, const string& heightmapPath, vec3 scale,
		const string& texturesPath = "");
	void LoadRandomTerrain(Node* thisNode, int rows, int columns, vec3 scale,
		const char* texturesPath = "");

	static ModelImporter* getInstance()
	{
		if (instance == NULL) instance = new ModelImporter();
		return instance;
	}
	ModelImporter(ModelImporter const&) = delete;
	void operator = (ModelImporter const&) = delete;

private:
	ModelImporter();
};