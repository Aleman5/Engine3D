#include "BoundingBox.h"
#include "Renderer.h"

BoundingBox::BoundingBox()
{
	Start();
}
BoundingBox::~BoundingBox()
{
}

void BoundingBox::Start()
{
	name = "BoundingBox";
	reqTransform = true;
}
void BoundingBox::Update()
{

}
void BoundingBox::Draw()
{

}
void BoundingBox::SetTransform(Transform* transform)
{
	this->transform = transform;
}

void BoundingBox::SetVertices(vec3 sVertices[CUBE_VERTICES])
{
	for (int i = 0; i < CUBE_VERTICES; i++)
	{
		this->vertices[i] = sVertices[i];

		if (vertices[i].x > maxs.x)
			maxs.x = vertices[i].x;
		if (vertices[i].y > maxs.y)
			maxs.y = vertices[i].y;
		if (vertices[i].z > maxs.z)
			maxs.z = vertices[i].z;

		if (vertices[i].x < mins.x)
			mins.x = vertices[i].x;
		if (vertices[i].y < mins.y)
			mins.y = vertices[i].y;
		if (vertices[i].z < mins.z)
			mins.z = vertices[i].z;
	}
}

void BoundingBox::SetVertices(vec4 sVertices[CUBE_VERTICES])
{
	for (int i = 0; i < CUBE_VERTICES; i++)
	{
		vertices[i] = sVertices[i];

		if (vertices[i].x > maxs.x)
			maxs.x = vertices[i].x;
		if (vertices[i].y > maxs.y)
			maxs.y = vertices[i].y;
		if (vertices[i].z > maxs.z)
			maxs.z = vertices[i].z;

		if (vertices[i].x < mins.x)
			mins.x = vertices[i].x;
		if (vertices[i].y < mins.y)
			mins.y = vertices[i].y;
		if (vertices[i].z < mins.z)
			mins.z = vertices[i].z;
	}
}

vec3 BoundingBox::GetVertexGlobalPosition(unsigned int index) const
{
	if (index > CUBE_VERTICES)
	{
		cerr << "WARNING: attempting to access an inexistent vertex." << endl;
		return vec3(0.0f);
	}

	vec4 globalRotatedPos(vertices[index].x, vertices[index].y, vertices[index].z, 1.0f);

	globalRotatedPos = Renderer::getInstance()->GetModelMatrix() * globalRotatedPos;

	return (vec3)globalRotatedPos;
}