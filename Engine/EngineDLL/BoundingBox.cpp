#include "BoundingBox.h"
#include "Renderer.h"

BoundingBox::BoundingBox() : maxs(vec3(-99999.0f, -99999.0f, -99999.0f)), mins(vec3(99999.0f, 99999.0f, 99999.0f))
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

void BoundingBox::SetVertice(int index, vec3 sVertice)
{
	vertices[index] = sVertice;

	if (vertices[index].x > maxs.x)
		maxs.x = vertices[index].x;
	if (vertices[index].y > maxs.y)
		maxs.y = vertices[index].y;
	if (vertices[index].z > maxs.z)
		maxs.z = vertices[index].z;

	if (vertices[index].x < mins.x)
		mins.x = vertices[index].x;
	if (vertices[index].y < mins.y)
		mins.y = vertices[index].y;
	if (vertices[index].z < mins.z)
		mins.z = vertices[index].z;
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