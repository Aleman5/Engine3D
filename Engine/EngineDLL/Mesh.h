#pragma once

#include <time.h> // Used for random purposes
#include "Component.h"
#include "Transform.h"
#include "Structs.h"
#include "ModelImporter.h"
#include "TextureImporter.h"

class ENGINEDLL_API Mesh : public Component
{
	Renderer* renderer;
	Material* material;
	Transform* transform;

	const string modelPath;
	const string sTexturePath;
	char* texturePath;

protected:
	vector<unsigned int> bufferTextures;
	vector<MeshEntry> m_Entries;
	vector<Header> m_Textures;
	FCCubeData fcData;

	void DrawFCData();

public:
	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;

	bool LoadMesh(const string& fileName, const string& textureName);
	

	Mesh(Material* material, const string modelPath, const string texturePath);
	~Mesh();
};
