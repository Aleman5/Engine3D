#include "Collider.h"

Collider::Collider() : geometry(NULL), transform(NULL)
{
	Start();
}

Collider::~Collider()
{
}

void Collider::Start()
{
	name = "Collider";
	reqTransform = true;
}
void Collider::Update()
{

}
void Collider::Draw()
{

}
void Collider::SetTransform(Transform* transform)
{
	this->transform = transform;
}