#include "Circle.h"

Circle::Circle(Renderer* renderer, Material* material, Layers tag, float radius, int totalTriangles) : Shape(renderer, material, tag)
{
	drawMode = GL_TRIANGLE_FAN;

	this->radius = radius;
	totTriangles = totalTriangles;

	variables = 3;
	
	count = (totTriangles + 2) * 3;
	degrees = 360.0f / totTriangles;
	angle = 0.0f;

	verticesData =		new float[count] {};
	verticesColorData = new float[count] {};
	glm::vec3 vec;

	verticesData[0] = 0;
	verticesData[1] = 0;
	verticesData[2] = 0;

	for (int i = 3; i < count; i += 3)
	{
		vec = glm::vec3(cos(angle), sin(angle), 0.0f) * radius;

		verticesData[i]	    = vec.x;
		verticesData[i + 1] = vec.y;
		verticesData[i + 2] = vec.z;
		angle += degrees * PI / 180.0f;

		cout << "Circle [" << i / 3 << "], angle: " << degrees * i / 3 << ", x: " << vec.x << ", y: " << vec.y << ", z: " << vec.z << endl;

		verticesColorData[i]     = 1.0f / (rand() % 10 + 1);
		verticesColorData[i + 1] = 1.0f / (rand() % 10 + 1);
		verticesColorData[i + 2] = 1.0f / (rand() % 10 + 1);
	}

	bufferId = SetVertices(verticesData, count / 3);
	colorBufferId = SetVertices(verticesColorData, count / 3);
}
Circle::~Circle()
{
	delete[] verticesColorData;
}

void Circle::Draw()
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