#include "Triangle.h"

Triangle::Triangle(Renderer* renderer, Material* material, Layers tag) : Shape(renderer, material, tag)
{
	verticesData = new float[9]{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	verticesColorData = new float[9]{
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
	};

	drawMode = GL_TRIANGLES;

	count = 3;
	variables = 3;
	bufferId = SetVertices(verticesData, count);
	colorBufferId = SetVertices(verticesColorData, count);
}
Triangle::~Triangle()
{
	delete[] verticesColorData;
}

void Triangle::Draw()
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
	renderer->BindBuffer(colorBufferId, 1);
	renderer->DrawBuffer(0, count, drawMode);
	renderer->DisableAttributes(0);
	renderer->DisableAttributes(1);
}