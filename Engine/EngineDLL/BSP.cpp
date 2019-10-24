#include "BSP.h"

BSP::BSP(const vec4& plane) : plane(plane)
{

}
BSP::~BSP()
{

}

void BSP::Start()
{
	name = "BSP";
	reqTransform = false;
}

void BSP::Update()
{
	actualHalfspace = Renderer::getInstance()->ClassifyPoint(plane, vec4(Renderer::getInstance()->GetCameraPosition(), 1.0f));
}

void BSP::Draw()
{

}

void BSP::SetTransform(Transform* transform)
{

}