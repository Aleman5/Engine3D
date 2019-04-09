#include "Camera.h"

Camera::Camera(Renderer* renderer, Material* material, Layers tag) : Entity(renderer, material, tag)
{
	vMatrix = renderer->GetViewMatrix();
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
	vMatrix = glm::translate(vMatrix, vec3(0.0f, 0.0f, mount));
	renderer->SetCameraPosition(vMatrix);
}

void Camera::Strafe(float mount)
{
	vMatrix = glm::translate(vMatrix, vec3(mount, 0.0f, 0.0f));
	renderer->SetCameraPosition(vMatrix);
}

void Camera::Pitch(float degrees)
{
	vMatrix = glm::rotate(vMatrix, degrees, vec3(-1.0f, 0.0f, 0.0f));
	renderer->SetCameraPosition(vMatrix);
}

void Camera::Yaw(float degrees)
{
	vMatrix = glm::rotate(vMatrix, degrees, vec3(0.0f, 1.0f, 0.0f));
	renderer->SetCameraPosition(vMatrix);
}

void Camera::Roll(float degrees)
{
	vMatrix = glm::rotate(vMatrix, degrees, vec3(0.0f, 0.0f, 1.0f));
	renderer->SetCameraPosition(vMatrix);
}