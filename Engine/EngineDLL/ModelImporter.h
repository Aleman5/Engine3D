#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "TextureImporter.h"
#include "Exports.h"
#include "Structs.h"
#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\glm.hpp>

using namespace std;

class Mesh;
class Node;
class Material;

class ENGINEDLL_API ModelImporter
{
	static ModelImporter *instance;

	const aiScene* scene = NULL;
	Assimp::Importer importer;
	
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