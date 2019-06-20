#include "Camera.h"

Camera::Camera(Renderer* renderer)
	: renderer(renderer)
{
	Start();
}
Camera::~Camera()
{
}

void Camera::Start()
{
	name = "Camera";
	reqTransform = false;

	right = vec4(1, 0, 0, 0);
	up = vec4(0, 1, 0, 0);
	forward = vec4(0, 0, 1, 0);
	pos = vec4(0, 0, -5, 1);

	vMatrix = lookAt(
		(vec3)pos,
		(vec3)(pos + forward),
		(vec3)up
	);

	renderer->SetCameraPosition(vMatrix);
}

void Camera::Update()
{
	
}

void Camera::Draw()
{

}

void Camera::SetTransform(Transform* transform)
{

}

void Camera::Walk(float mountX, float mountZ)
{
	pos = glm::translate(mat4(1.0f), (vec3)((right * mountX) + (forward * mountZ))) * pos;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + forward),
		(vec3)up
	);

	renderer->SetCameraPosition(vMatrix);
}

void Camera::Pitch(float degrees)
{
	forward = rotate(mat4(1.0f), degrees, vec3(right.x, right.y, right.z)) * forward;
	up = rotate(mat4(1.0f), degrees, vec3(right.x, right.y, right.z)) * up;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + forward),
		(vec3)up
	);

	renderer->SetCameraPosition(vMatrix);
}

void Camera::Yaw(float degrees)
{
	forward = rotate(mat4(1.0f), degrees, vec3(up.x, up.y, up.z)) * forward;
	right = rotate(mat4(1.0f), degrees, vec3(up.x, up.y, up.z)) * right;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + forward),
		(vec3)up
	);

	renderer->SetCameraPosition(vMatrix);
}

void Camera::Roll(float degrees)
{
	right = rotate(mat4(1.0f), degrees, vec3(forward.x, forward.y, forward.z)) * right;
	up = rotate(mat4(1.0f), degrees, vec3(forward.x, forward.y, forward.z)) * up;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + forward),
		(vec3)up
	);

	renderer->SetCameraPosition(vMatrix);
}