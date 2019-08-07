#include "Box.h"

Box::Box()
{
	Start();
}
Box::~Box()
{
}

void Box::Start()
{
	name = "Box";
	reqTransform = true;
}

void Box::Update()
{

}

void Box::Draw()
{

}

void Box::SetTransform(Transform* transform)
{
	this->transform = transform;
}