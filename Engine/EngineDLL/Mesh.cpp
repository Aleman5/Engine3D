#include "Mesh.h"

Mesh::Mesh(Renderer* renderer, Material* material, Layers tag) : Entity(renderer, material, tag)
{
	srand(time(0));
}
Mesh::~Mesh()
{
}

void Mesh::Update()
{
	Entity::Update();
}

void Mesh::ShouldDispose()
{
	if (shouldDispose)
	{
		renderer->DestroyBuffer(bufferId);
		delete[] verticesData;
		shouldDispose = false;
	}
}

unsigned int Mesh::SetVertices(float* vertices, int count)
{
	verticesData = vertices;

	unsigned int id = renderer->GenBuffer(verticesData, sizeof(float) * count * variables);
	shouldDispose = true;

	return id;
}