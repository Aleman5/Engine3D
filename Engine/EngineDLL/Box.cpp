#include "Box.h"

Box::Box()
{
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