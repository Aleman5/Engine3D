#include "Transform.h"

Transform::Transform()
{
	Start();
}
Transform::~Transform()
{
}

void Transform::Start()
{
	name = "Transform";
	reqTransform = false;

	SetLayer(Default);
	isStatic = false;
	mass = 1.0f;

	vectorPosition = vectorRotation = vectorScale = vec3(0.0f);

	model = lTransMat = wTransMat =
	lRotX = lRotY = lRotZ =
	wRotX = wRotY = wRotZ =
	lScaleMat = wScaleMat = mat4(1.0f);

	UpdateModel();
}
void Transform::Update()
{
	if (collision) collision = NULL;

	UpdateModel();
}
void Transform::Draw()
{

}
void Transform::SetTransform(Transform* transform)
{

}

void Transform::UpdateModel()
{
	model = lTransMat * lRotX * lRotY * lRotZ * lScaleMat;
}

void Transform::Translate(vec3 vector3)
{
	vectorPosition += vector3;

	lTransMat *= glm::translate(mat4(1.0f), vectorPosition);

	UpdateModel();
}

void Transform::Translate(float newX, float newY, float newZ)
{
	vectorPosition += vec3(newX, newY, newZ);

	lTransMat *= glm::translate(mat4(1.0f), vectorPosition);

	UpdateModel();
}

void Transform::Teleport(float newX, float newY, float newZ)
{
	vectorPosition = vec3(newX, newY, newZ);

	lTransMat = glm::translate(mat4(1.0f), vectorPosition);

	UpdateModel();
}

void Transform::Scale(vec3 vector3)
{
	lScaleMat *= glm::scale(vector3);

	vectorScale += vector3;

	UpdateModel();
}

void Transform::Scale(float newX, float newY, float newZ)
{
	vec3 vector3(newX, newY, newZ);

	lScaleMat *= glm::scale(vector3);

	vectorScale += vector3;

	UpdateModel();
}

void Transform::RotateX(float angle)
{
	vec3 vecAxis;

	vecAxis[1] = vecAxis[2] = 0.0f;
	vecAxis[0] = 1.0f;

	lRotX *= glm::rotate(mat4(1.0f), glm::radians(angle), vecAxis);

	vectorRotation += vecAxis;

	UpdateModel();
}

void Transform::RotateY(float angle)
{
	vec3 vecAxis;

	vecAxis[0] = vecAxis[2] = 0.0f;
	vecAxis[1] = 1.0f;

	lRotY *= glm::rotate(mat4(1.0f), glm::radians(angle), vecAxis);

	vectorRotation += vecAxis;

	UpdateModel();
}

void Transform::RotateZ(float angle)
{
	vec3 vecAxis;

	vecAxis[0] = vecAxis[1] = 0.0f;
	vecAxis[2] = 1.0f;

	lRotZ *= glm::rotate(mat4(1.0f), glm::radians(angle), vecAxis);

	vectorRotation += vecAxis;

	UpdateModel();
}

void Transform::SetLayer(Layers layer)
{
	this->layer = layer;
}