#include "Mesh.h"

Mesh::Mesh(Renderer* renderer, Material* material, Layers tag, const string modelPath) : Entity(renderer, material, tag)
{
	srand(time(0));

	count = 8;
	variables = 3;

	ModelImporter::getInstance()->Import3DFromFile(modelPath);

	float* vertices = new float[count * variables]{
		// Front
		-1.0, -1.0, 1.0,
		1.0, -1.0, 1.0,
		1.0, 1.0, 1.0,
		-1.0, 1.0, 1.0,
		// Back
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0, 1.0, -1.0,
		-1.0, 1.0, -1.0,
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

	verticesColorData = new float[count * variables]{
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
		0.234f, 0.125f, 0.753f,
		0.852f, 0.199f, 0.254f,
		0.578f, 0.985f, 0.025f,
		0.747f, 0.695f, 0.141f,
	};

	indices = tempIndices;

	colorBufferId = SetVertices(verticesColorData, count);
	bufferId = SetVertices(vertices, count);
	bufferIndices = renderer->GenElementBuffer(indices, NULL, 0);
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
	//unsigned int id = renderer->GenBuffer(indices, verticesData, sizeof(float) * count * variables);
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
	renderer->EnableAttributes(1);
	renderer->BindBuffer(bufferId, 0);
	renderer->BindTextureBuffer(colorBufferId, 1);
	renderer->BindElementBuffer(bufferIndices);
	//renderer->DrawBuffer(0, count, drawMode);
	renderer->DrawElementBuffer(indices);
	renderer->DisableAttributes(0);
	renderer->DisableAttributes(1);
}