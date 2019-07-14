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

using namespace std;

class ENGINEDLL_API Mesh : public Component
{
	Renderer* renderer;
	Material* material;
	Transform* transform;

	bool debugMode;
	void DrawFCData();

public:
	vector<unsigned int> bufferTextures;
	vector<MeshEntry> m_Entries;
	vector<Header> m_Textures;
	FCCubeData fcData;

	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;
	
	void ActivateDebugMode();
	void DesactivateDebugMode();

	Mesh(Material* material);
	~Mesh();
};