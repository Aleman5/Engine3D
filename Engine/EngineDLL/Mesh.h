#pragma once

#include <time.h> // Used for random purposes
#include "Entity.h"

class ENGINEDLL_API Mesh : public Entity
{
protected:
	unsigned int bufferIndices;

	float* verticesData;	// Data of the Vertices
	int count;				// Total vertices
	int variables;			// Total data for each vertice

	vector<unsigned int> indices;

	

public:
	void Draw() override;
	void ShouldDispose() override;

	unsigned int SetVertices(
		float* vertices,	// Data of the vertices
		int count			// Total Vertices
	) override;

	void Update();

	Mesh(Renderer* renderer, Material* material, Layers tag);
	~Mesh();
};
