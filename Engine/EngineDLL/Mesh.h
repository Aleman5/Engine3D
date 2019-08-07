#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Exports.h"
#include "Component.h"
#include "Material.h"
#include "Transform.h"
#include "Structs.h"
#include "TextureImporter.h"
#include "GlobalDefs.h"

using namespace std;

class ENGINEDLL_API Mesh : public Component
{
	Renderer* renderer;
	Material* material;
	Transform* transform;

	bool debugMode;
public:
	vector<unsigned int> bufferTextures;
	vector<MeshEntry> m_Entries;
	vector<Header> m_Textures;

	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;
	
	void DrawFCData(const FCCubeData& fcData);

	void SetDiffTex(vector<Header> diffuseTextures = vector<Header>());

	void ActivateDebugMode();
	void DesactivateDebugMode();

	bool GetDebugMode();

	Mesh();
	~Mesh();
};