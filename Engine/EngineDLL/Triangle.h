#pragma once

#include "Shape.h"

class ENGINEDLL_API Triangle : public Shape
{
	float* verticesColorData;	// Data of the Color Vertices
	unsigned int colorBufferId;	// Id of the Color Buffer

public:
	void Draw() override;

	Triangle(Renderer* renderer, Material* material, Layers tag);
	~Triangle();
};

