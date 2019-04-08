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
	//vectorPosition.z += mount;
	//vectorPosition.z += mount;

	GetRenderer()->SetCameraPosition(vec3(0.0f, 0.0f, mount));

	vec3 pos = GetRenderer()->GetCameraPosition();

	cout << "x: " << pos.x << " y: " << pos.y << " z: " << pos.z << endl;
}

void Camera::Strafe(float mount)
{
	GetRenderer()->SetCameraPosition(vec3(mount, 0.0f, 0.0f));

	vec3 pos = GetRenderer()->GetCameraPosition();

	cout << "x: " << pos.x << " y: " << pos.y << " z: " << pos.z << endl;
}

void Camera::Pitch(float degrees)
{
	GetRenderer()->RotateCamera(vec3(0, degrees, 0));
}

void Camera::Yaw(float degrees)
{
	GetRenderer()->RotateCamera(vec3(degrees, 0, 0));
}

void Camera::Roll(float degrees)
{
	GetRenderer()->RotateCamera(vec3(0, 0, degrees));
}