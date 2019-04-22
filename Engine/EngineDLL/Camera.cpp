#include "Camera.h"

Camera::Camera(Renderer* renderer)
{
	this->renderer = renderer;
	
	x = vec4(1, 0, 0, 0); // Right
	y = vec4(0, 1, 0, 0); // Up
	z = vec4(0, 0, 1, 0); // Forward
	pos = vec4(0, 0, -5, 1);

	vMatrix = lookAt(
		(vec3)pos,
		(vec3)(pos + z),
		(vec3)y
	);

	renderer->SetCameraPosition(vMatrix);
}
Camera::~Camera()
{
}

void Camera::Walk(float mount)
{
	pos = glm::translate(mat4(1.0f), (vec3)(z * mount)) * pos;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + z),
		(vec3)y
	);

	renderer->SetCameraPosition(vMatrix);

	/*vMatrix = glm::translate(vMatrix, vec3(0.0f, 0.0f, mount));
	renderer->SetCameraPosition(vMatrix);*/
}

void Camera::Strafe(float mount)
{
	pos = glm::translate(mat4(1.0f), (vec3)(x * mount)) * pos;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + z),
		(vec3)y
	);

	renderer->SetCameraPosition(vMatrix);

	/*vMatrix = glm::translate(vMatrix, vec3(mount, 0.0f, 0.0f));
	renderer->SetCameraPosition(vMatrix);*/
}

void Camera::Pitch(float degrees)
{
	/*z = rotate(mat4(1.0f), degrees, vec3(0.0f, 1.0f, 0.0f)) * forward;
	x = rotate(mat4(1.0f), degrees, vec3(0.0f, 1.0f, 0.0f)) * right;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + z),
		(vec3)y
	);

	renderer->SetCameraPosition(vMatrix);*/

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