#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Exports.h"
#include "Node.h"
#include "Component.h"
#include "Transform.h"
#include "Structs.h"
#include "TextureImporter.h"


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

class ENGINEDLL_API Mesh : public Component
{
	const aiScene* scene = NULL;
	Assimp::Importer importer;
	bool InitFromScene(const aiScene* pScene);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void InitTexture();

	Renderer* renderer;
	Material* material;
	Transform* transform;

	bool debugMode;
	void DrawFCData();

public:
	const string modelPath;
	const string sTexturePath;
	char* texturePath;
	vector<unsigned int> bufferTextures;
	vector<MeshEntry> m_Entries;
	vector<Header> m_Textures;
	FCCubeData fcData;

	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;

	bool Import3DFromFile();
	
	void ActivateDebugMode();
	void DesactivateDebugMode();

	Mesh(Material* material, const string modelPath, const string texturePath);
	~Mesh();
};