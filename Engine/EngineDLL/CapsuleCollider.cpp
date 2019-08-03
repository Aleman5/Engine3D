#include "CapsuleCollider.h"



CapsuleCollider::CapsuleCollider()
{
}


CapsuleCollider::~CapsuleCollider()
{
}

void CapsuleCollider::Start()
{
	name = "CapsuleCollider";
	reqTransform = true;
}
void CapsuleCollider::Update()
{

}
void CapsuleCollider::Draw()
{

}
void CapsuleCollider::SetTransform(Transform* transform)
{
	this->transform = transform;
}

void CapsuleCollider::CreateGeometry(BoundingBox* boundingBox)
{
	float radius, halfHeight;

	vec3 maxs = boundingBox->GetMaxs();
	vec3 mins = boundingBox->GetMins();

	radius = (maxs.x - mins.x) * 0.5f;
	halfHeight = (maxs.y - mins.y) * 0.5f;

	float scaleX = transform->GetScale().x;
	float scaleY = transform->GetScale().y;

	geometry = new PxCapsuleGeometry(radius * scaleX, halfHeight * scaleY);
}

void CapsuleCollider::CreateGeometry(float radius, float height)
{
	geometry = new PxCapsuleGeometry(radius, height * 0.5f);
}