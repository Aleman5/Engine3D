#include "Mesh.h"

Mesh::Mesh(Renderer* renderer, Material* material, Layers tag) : Entity(renderer, material, tag)
{
	srand(time(0));

	count = 8;
	variables = 3;

	float* vertices = new float[count * variables]{
		// Front
		-1.0, -1.0,  1.0,
		1.0, -1.0,  1.0,
		1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// Back
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0,
	};

	vector<unsigned int> tempIndices {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3,
	};

	indices = tempIndices;

	bufferId = SetVertices(vertices, count);
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

	//unsigned int id = renderer->GenBuffer(verticesData, sizeof(float) * count * variables);
	unsigned int id = renderer->GenElementBuffer(indices, verticesData, sizeof(float) * count * variables);
	shouldDispose = true;

	return id;
}

void Mesh::Draw()
{
	renderer->loadIdentityMatrix();
	renderer->SetModelMatrix(model);

	if (material != NULL)
	{
		material->Bind();
		material->SetMatrixProperty("MVP", renderer->GetMVP());
	}

	renderer->EnableAttributes(0);
	//renderer->EnableAttributes(1);
	//renderer->BindBuffer(bufferId, 0);
	//renderer->BindTextureBuffer(uvBufferId, 1);
	renderer->BindElementBuffer(bufferId, indices);
	//renderer->DrawBuffer(0, count, drawMode);
	renderer->DisableAttributes(0);
	//renderer->DisableAttributes(1);
}