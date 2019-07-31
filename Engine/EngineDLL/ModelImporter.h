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
class Material;
class aiScene;
class aiNode;
class aiMesh;

class ENGINEDLL_API ModelImporter
{
	static ModelImporter *instance;

	bool AttendNode(const aiScene* aiScene, aiNode* aiNode, Node* parent,
					FCCubeData& fcData, const string modelPath, const string texturePath, Material* material);
	Mesh* InitMesh(const aiScene* aiScene, const aiMesh* aiMesh, Node* parent,
				   FCCubeData& fcData, const string modelPath, const string texturePath, unsigned int Index, Material* material);
	void InitTexture();

public:
	void Load(Node* thisNode, const string modelPath, const string texturePath, Material* material);

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