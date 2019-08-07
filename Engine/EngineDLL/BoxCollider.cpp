#include "BoxCollider.h"

BoxCollider::BoxCollider()
{
	Start();
}
BoxCollider::~BoxCollider()
{
}

void BoxCollider::Start()
{
	name = "BoxCollider";
	reqTransform = true;
}
void BoxCollider::Update()
{

}
void BoxCollider::Draw()
{

}
void BoxCollider::SetTransform(Transform* transform)
{
	this->transform = transform;
}

void BoxCollider::CreateGeometry(BoundingBox* boundingBox)
{
	float halfWidth, halfHeight, halfDepth;

	vec3 maxs = boundingBox->GetMaxs();
	vec3 mins = boundingBox->GetMins();

	halfWidth = (maxs.x - mins.x) * 0.5f;
	halfHeight = (maxs.y - mins.y) * 0.5f;
	halfDepth = (maxs.z - mins.z) * 0.5f;

	float scaleX = transform->GetScale().x;
	float scaleY = transform->GetScale().y;
	float scaleZ = transform->GetScale().z;

	geometry = new PxBoxGeometry(halfHeight * scaleY, halfWidth * scaleX, halfDepth * scaleZ);
}

void BoxCollider::CreateGeometry(float width, float height, float depth)
{
	geometry = new PxBoxGeometry(height * 0.5f, width * 0.5f, depth * 0.5f);
}