#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "TextureImporter.h"
#include "Exports.h"
#include "Definitions.h"
#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"
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
	
	void Clear(vector<Header*>& m_Textures);

	bool InitFromScene(const aiScene* pScene, const string& Filename, const string& texturePath, vector<MeshEntry>& m_Entries, vector<Header*>& m_Textures, Renderer* renderer);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh, vector<MeshEntry>& m_Entries, Renderer* renderer);
	bool InitMaterials(const aiScene* pScene, const string& Filename, const string& texturePath, vector<Header*>& m_Textures);

public:
	bool Import3DFromFile(const string& modelPath, const string& texturePath, vector<MeshEntry>& m_Entries, vector<Header*>& m_Textures, Renderer* renderer);

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

