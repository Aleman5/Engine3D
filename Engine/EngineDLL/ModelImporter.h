/*#pragma once

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

class ENGINEDLL_API ModelImporter
{
	static ModelImporter *instance;

	const aiScene* scene = NULL;
	Assimp::Importer importer;
	
	bool InitFromScene(const aiScene* pScene, Mesh* mesh);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh, Mesh* mesh);

public:
	bool Import3DFromFile(Mesh* mesh);

	static ModelImporter* getInstance()
	{
		if (instance == NULL) instance = new ModelImporter();
		return instance;
	}
	ModelImporter(ModelImporter const&) = delete;
	void operator = (ModelImporter const&) = delete;

private:
	ModelImporter();
};*/