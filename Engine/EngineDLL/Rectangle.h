#pragma once

#include "Shape.h"

class ENGINEDLL_API Rectangle : public Shape
{
	float* verticesColorData; // Data of the Color Vertices

public:

	void Draw() override;

	Rectangle(Renderer* renderer, Material* material, Layers tag);
	~Rectangle();
};