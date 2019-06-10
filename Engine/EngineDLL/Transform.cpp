#include "Transform.h"

Transform::Transform(Renderer* renderer, Layers tag)
{
	this->renderer = renderer;
	this->tag = tag;
	isStatic = false;

	vectorPosition = vectorRotation = vectorScale = vec3(0.0f);

	model = mat4(1.0f);
	localTransMatrix = mat4(1.0f);
	worldTransMatrix = mat4(1.0f);
	rotateX = mat4(1.0f);
	rotateY = mat4(1.0f);
	rotateZ = mat4(1.0f);
	scallingMatrix = mat4(1.0f);

	mass = 1.0f;

	UpdateModel();

	collision = NULL;
}
Transform::~Transform()
{
}

void Transform::Update()
{
	if (collision) collision = NULL;
	UpdateModel();
}

void Transform::UpdateModel()
{
	model = worldTransMatrix * rotateX * rotateY * rotateZ * scallingMatrix;
}

void Transform::Translate(vec3 vector3)
{
	vectorPosition += vector3;

	worldTransMatrix *= glm::translate(mat4(1.0f), vectorPosition);

	UpdateModel();
}

void Transform::Translate(float newX, float newY, float newZ)
{
	vectorPosition += vec3(newX, newY, newZ);

	worldTransMatrix *= glm::translate(mat4(1.0f), vectorPosition);

	UpdateModel();
}

void Transform::Teleport(float newX, float newY, float newZ)
{
	vectorPosition = vec3(newX, newY, newZ);

	worldTransMatrix = glm::translate(mat4(1.0f), vectorPosition);

	UpdateModel();
}

void Transform::Scale(vec3 vector3)
{
	scallingMatrix *= glm::scale(vector3);

	vectorScale += vector3;

	UpdateModel();
}

void Transform::Scale(float newX, float newY, float newZ)
{
	vec3 vector3(newX, newY, newZ);

	scallingMatrix *= glm::scale(vector3);

	vectorScale += vector3;

	UpdateModel();
}

void Transform::RotateX(float angle)
{
	vec3 vecAxis;

	vecAxis[1] = vecAxis[2] = 0.0f;
	vecAxis[0] = 1.0f;

	rotateX *= glm::rotate(mat4(1.0f), glm::radians(angle), vecAxis);

	vectorRotation += vecAxis;

	UpdateModel();
}

void Transform::RotateY(float angle)
{
	vec3 vecAxis;

	vecAxis[0] = vecAxis[2] = 0.0f;
	vecAxis[1] = 1.0f;

	rotateY *= glm::rotate(mat4(1.0f), glm::radians(angle), vecAxis);

	vectorRotation += vecAxis;

	UpdateModel();
}

void Transform::RotateZ(float angle)
{
	vec3 vecAxis;

	vecAxis[0] = vecAxis[1] = 0.0f;
	vecAxis[2] = 1.0f;

	rotateZ *= glm::rotate(mat4(1.0f), glm::radians(angle), vecAxis);

	vectorRotation += vecAxis;

	UpdateModel();
}