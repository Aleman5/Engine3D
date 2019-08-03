#include "Rectangle.h"

Rectangle::Rectangle(Renderer* renderer, Material* material, Layers tag) : Shape(renderer, material, tag)
{
	verticesData = new float[12]{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	verticesColorData = new float[12]{
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
	};

	drawMode = GL_TRIANGLE_STRIP;

	count = 4;
	variables = 3;
	bufferId = SetVertices(verticesData, count);
	colorBufferId = SetVertices(verticesColorData, count);
}

Rectangle::~Rectangle()
{
	delete[] verticesColorData;
}

void Rectangle::Draw()
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
	renderer->DrawBuffer(count, drawMode);
	renderer->DisableAttributes(0);
	renderer->DisableAttributes(1);
}