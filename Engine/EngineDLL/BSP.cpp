#include "BSP.h"
#include "Transform.h"

BSP::BSP(const vec3& direction) : renderer(Renderer::getInstance()), direction(direction)
{

}
BSP::~BSP()
{

}

void BSP::Start()
{
	name = "BSP";
	reqTransform = true;

	position = transform->GetGlobalPosition();
	plane = renderer->CreatePlane(direction, position);
	renderer->NormalizePlane(plane);
}

void BSP::Update()
{
	halfspace = renderer->ClassifyPoint(plane, vec4(renderer->GetCameraPosition(), 1.0f));

	if (position != transform->GetGlobalPosition())
	{
		position = transform->GetGlobalPosition();
		plane = renderer->CreatePlane(direction, position);
		renderer->NormalizePlane(plane);
	}
}

void BSP::Draw()
{

}

void BSP::SetTransform(Transform* transform)
{
	this->transform = transform;
}