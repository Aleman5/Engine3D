#pragma once

#include <time.h> // Used for random purposes
#include "Entity.h"

class ENGINEDLL_API Mesh : public Entity
{
protected:
	float* verticesData;	// Data of the Vertices
	int count;				// Total vertices
	int variables;			// Total data for each vertice

	void Update();

public:
	virtual void Draw() = 0;
	void ShouldDispose() override;

	unsigned int SetVertices(
		float* vertices,	// Data of the vertices
		int count			// Total Vertices
	) override;

	Mesh(Renderer* renderer, Material* material, Layers tag);
	~Mesh();
};
