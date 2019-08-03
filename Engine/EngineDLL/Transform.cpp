#include "Transform.h"

Transform::Transform() : layer(Default), parent(NULL), collision(NULL)
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

	right = vec3(1, 0, 0);
	up	  = vec3(0, 1, 0);
	fwd   = vec3(0, 0, 1);

	right = vec3(1, 0, 0);
	up	  = vec3(0, 1, 0);
	fwd   = vec3(0, 0, 1);

	model = lTransMat = lRot = lScaleMat = mat4(1.0f);

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
	model = lTransMat * lRot * lScaleMat;
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
	vectorRotation.x += angle;

	UpdateRotationMatrix();
	UpdateModel();
}

void Transform::RotateY(float angle)
{
	vectorRotation.y += angle;

	UpdateRotationMatrix();
	UpdateModel();
}

void Transform::RotateZ(float angle)
{
	vectorRotation.z += angle;

	UpdateRotationMatrix();
	UpdateModel();
}

void Transform::Rotate(float x, float y, float z)
{
	vec3 rot = vec3(x, y, z);
	vectorRotation += rot;

	UpdateRotationMatrix();
	UpdateModel();
}

void Transform::SetRotation(float x, float y, float z)
{
	vectorRotation = vec3(x, y, z);

	ClampEulerRotation();
	UpdateRotationMatrix();
	UpdateDirectionVectors();

	UpdateModel();
}

void Transform::ChangeRotationMatrix(vec4 quaternion)
{
	float pitch, yaw, roll;

	quaternion = normalize(quaternion);

	ConvertToEulerAngles(quaternion, pitch, yaw, roll);

	vectorRotation = vec3(pitch, yaw, roll);

	mat4 mat1 = mat4
	{
		quaternion.w, quaternion.z, -quaternion.y, quaternion.x,
		-quaternion.z, quaternion.w, quaternion.x, quaternion.y,
		quaternion.y, -quaternion.x, quaternion.w, quaternion.z,
		-quaternion.x, -quaternion.y, -quaternion.z, quaternion.w
	};

	mat4 mat2 = mat4
	{
		quaternion.w, quaternion.z, -quaternion.y, -quaternion.x,
		-quaternion.z, quaternion.w, quaternion.x, -quaternion.y,
		quaternion.y, -quaternion.x, quaternion.w, -quaternion.z,
		quaternion.x, quaternion.y, quaternion.z, quaternion.w
	};

	lRot = mat1 * mat2;

	ClampEulerRotation();
	UpdateDirectionVectors();
	UpdateModel();
}

void Transform::ClampEulerRotation()
{
	if (vectorRotation.x < 0.0f || vectorRotation.x >= FULL_ROTATION)
		vectorRotation.x = vectorRotation.x - (floor(vectorRotation.x / FULL_ROTATION) * FULL_ROTATION);

	if (vectorRotation.y < 0.0f || vectorRotation.y >= FULL_ROTATION)
		vectorRotation.y = vectorRotation.y - (floor(vectorRotation.y / FULL_ROTATION) * FULL_ROTATION);

	if (vectorRotation.z < 0.0f || vectorRotation.z >= FULL_ROTATION)
		vectorRotation.z = vectorRotation.z - (floor(vectorRotation.z / FULL_ROTATION) * FULL_ROTATION);
}

void Transform::UpdateDirectionVectors()
{
	vec4 idenRight  (1.0f, 0.0f, 0.0f, 0.0f);
	vec4 idenUp		(0.0f, 1.0f, 0.0f, 0.0f);
	vec4 idenForward(0.0f, 0.0f, 1.0f, 0.0f);

	right	= normalize((vec3)(lRot	* idenRight	 ));
	up		= normalize((vec3)(lRot	* idenUp	 ));
	fwd		= normalize((vec3)(lRot	* idenForward));

	lRight	= normalize((vec3)(idenRight   * lRot));
	lUp		= normalize((vec3)(idenUp	   * lRot));
	lFwd	= normalize((vec3)(idenForward * lRot));
}

void Transform::UpdateRotationMatrix()
{
	mat4 rotationX = rotate(mat4(1.0f), radians(vectorRotation.x), vec3(1.0f, 0.0f, 0.0f));
	mat4 rotationY = rotate(mat4(1.0f), radians(vectorRotation.y), vec3(0.0f, 1.0f, 0.0f));
	mat4 rotationZ = rotate(mat4(1.0f), radians(vectorRotation.z), vec3(0.0f, 0.0f, 1.0f));

	lRot = rotationX * rotationY * rotationZ;
}

void Transform::ConvertToEulerAngles(const vec4& quaternion, float& pitch, float& yaw, float& roll)
{
	float sinPitchCosYaw = 2.0f * (quaternion.w * quaternion.x + quaternion.y * quaternion.z);
	float cosPitchCosYaw = 1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y);
	pitch = atan(sinPitchCosYaw, cosPitchCosYaw);

	float sinYaw = 2.0f * (quaternion.w * quaternion.y - quaternion.z * quaternion.x);
	yaw = (abs(sinYaw) >= 1.0f) ? sign(sinYaw) * half_pi<float>() : asin(sinYaw);

	float sinRollCosYaw = 2.0f * (quaternion.w * quaternion.z + quaternion.x * quaternion.y);
	float cosRollCosYaw = 1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
	roll = atan(sinRollCosYaw, cosRollCosYaw);

	pitch = degrees(pitch);
	yaw	  = degrees(yaw);
	roll  = degrees(roll);
}

vec4 Transform::ConvertToQuaternion(float pitch, float yaw, float roll)
{
	vec4 rotQuat;

	float cosPitch = (float)cos(radians(pitch) * 0.5);
	float sinPitch = (float)sin(radians(pitch) * 0.5);
	float cosYaw = (float)cos(radians(yaw) * 0.5);
	float sinYaw = (float)sin(radians(yaw) * 0.5);
	float cosRoll = (float)cos(radians(roll) * 0.5);
	float sinRoll = (float)sin(radians(roll) * 0.5);

	rotQuat.w = cosRoll * cosYaw * cosPitch + sinRoll * sinYaw * sinPitch;
	rotQuat.x = cosRoll * cosYaw * sinPitch - sinRoll * sinYaw * cosPitch;
	rotQuat.y = sinRoll * cosYaw * sinPitch + cosRoll * sinYaw * cosPitch;
	rotQuat.z = sinRoll * cosYaw * cosPitch - cosRoll * sinYaw * sinPitch;

	return rotQuat;
}

void Transform::SetLayer(Layers layer)
{
	this->layer = layer;
}

void Transform::SetParent(Transform* parent)
{
	this->parent = parent;
}

void Transform::SetGlobalPosition(float x, float y, float z)
{
	vec3 globalPosition = GetGlobalPosition();
	vec3 targetGloabalPosition = vec3(x, y, z);
	vec3 globalDiff = targetGloabalPosition - globalPosition;

	Teleport(vectorPosition.x + globalDiff.x, vectorPosition.y + globalDiff.y, vectorPosition.z + globalDiff.z);
}

void Transform::SetGlobalRotation(float x, float y, float z)
{
	vec3 globalRotation = GetGlobalPosition();
	vec3 targetGloabalRotation = vec3(x, y, z);
	vec3 globalDiff = targetGloabalRotation - globalRotation;

	Teleport(vectorRotation.x + globalDiff.x, vectorRotation.y + globalDiff.y, vectorRotation.z + globalDiff.z);
}

vec3 Transform::GetGlobalPosition()
{
	vec3 globalPosition = vectorPosition;

	Transform* parentTransform = parent;

	while (parentTransform)
	{
		globalPosition += parentTransform->GetPosition();
		parentTransform = parentTransform->GetParent();
	}

	return globalPosition;
}

vec3 Transform::GetGlobalRotation()
{
	vec3 globalRotation = vectorRotation;

	Transform* parentTransform = parent;

	while (parentTransform)
	{
		globalRotation += parentTransform->GetRotation();
		parentTransform = parentTransform->GetParent();
	}

	return globalRotation;
}