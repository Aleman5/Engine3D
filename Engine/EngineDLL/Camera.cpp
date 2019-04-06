#include "Camera.h"

Camera::Camera(Renderer* renderer, Material* material, Layers tag) : Entity(renderer, material, tag)
{
	vectorPosition = renderer->GetCameraPosition();
}
Camera::~Camera()
{
}
void Camera::Draw()
{

}
void Camera::ShouldDispose()
{

}
unsigned int Camera::SetVertices(float* vertices, int count)
{
	return NULL;
}

void Camera::Walk(float mount)
{
	vectorPosition.z += mount;

	GetRenderer()->SetCameraPosition(vec3(0.0f, 0.0f, mount));


}

void Camera::Strafe(float mount)
{

}