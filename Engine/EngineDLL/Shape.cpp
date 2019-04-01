#include "Shape.h"

Shape::Shape(Renderer* renderer, Material* material, Layers tag) : Entity(renderer, material, tag)
{
	srand(time(0));
}
Shape::~Shape()
{
}

void Shape::Update()
{
	Entity::Update();
}

void Shape::ShouldDispose()
{
	if (shouldDispose)
	{
		renderer->DestroyBuffer(bufferId);
		delete[] verticesData;
		shouldDispose = false;
	}
}

unsigned int Shape::SetVertices(float* vertices, int count)
{
	verticesData = vertices;

	unsigned int id = renderer->GenBuffer(verticesData, sizeof(float) * count * variables);
	shouldDispose = true;

	return id;
}