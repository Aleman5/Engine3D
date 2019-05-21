#pragma once

#include <time.h> // Used for random purposes
#include "Entity.h"
#include "Structs.h"
#include "ModelImporter.h"
#include "TextureImporter.h"

class ENGINEDLL_API Mesh : public Entity
{
protected:
	vector<unsigned int> bufferTextures;
	char* texturePath;
	vector<MeshEntry> m_Entries;
	vector<Header> m_Textures;

public:
	void Draw() override;
	void ShouldDispose() override;

	unsigned int SetVertices(
		float* vertices,	// Data of the vertices
		int count			// Total Vertices
	) override;

	void Update();

	bool LoadMesh(const string& fileName, const string& textureName);
	

	Mesh(Renderer* renderer, Material* material, Layers tag, string modelPath, string texturePath);
	~Mesh();
};
