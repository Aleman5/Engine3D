#pragma once

#include "Shape.h"
#define PI 3.14159265359f

class ENGINEDLL_API Circle : public Shape
{
	float* verticesColorData;// Data of the Color Vertices
	float radius;			 // Radius of the Circle
	float degrees;			 // Degrees
	float angle;			 // Angle
	int totTriangles;		 // Total of Triangles

public:
	void Draw() override;

	Circle(Renderer* renderer, Material* material, Layers tag, float radius, int totalTriangles);
	~Circle();
};

