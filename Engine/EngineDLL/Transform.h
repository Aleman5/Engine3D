#pragma once

#include "GL\glew.h"
#include "GLFW\glfw3.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\glm.hpp>

using namespace glm;

#include "Component.h"

static enum Space
{
	Local,
	World
};

class ENGINEDLL_API Transform : public Component
{
protected:
	vec3 vectorPosition;	// Actual Position
	vec3 vectorRotation;	// Actual Rotation
	vec3 vectorScale;		// Actual Scale

	mat4 model;				// Model Matrix

	mat4 localTransMatrix;	// Translation Matrix
	mat4 worldTransMatrix;	// Translation Matrix
	mat4 rotateX;			// RotationX Matrix
	mat4 rotateY;			// RotationX Matrix
	mat4 rotateZ;			// RotationX Matrix
	mat4 scallingMatrix;	// Scale Matrix

public:
	void Start() override;
	void Update() override;
	void Draw() override;
	
	void UpdateModel();
	void Translate(
		vec3 vector3// Vector3 to Translate
	);
	void Translate(
		float newX,	// Value in X
		float newY,	// Value in Y
		float newZ	// Value in Z
	);
	void Teleport(
		float newX, // Value in X
		float newY,	// Value in Y
		float newZ	// Value in Z
	);
	void Scale(
		vec3 vector3// Vector3 to Scale
	);
	void Scale(
		float newX, // Value in X
		float newY, // Value in Y
		float newZ  // Value in Z
	);
	void RotateX(
		float angle // Value in X axis
	);
	void RotateY(
		float angle // Value in Y axis
	);
	void RotateZ(
		float angle // Value in Z axis
	);

	Transform* GetTransform() { return this; }		// Returns a pointer to this Entity
	vec3 GetPosition() { return vectorPosition; }	// Returns the actual position
	vec3 GetScale()	{ return vectorScale; }			// Returns the actual scale
	vec3 GetRotation() { return vectorRotation; }	// Returns the actual rotation
	mat4 GetLocalMatrix() { return localTransMatrix; } // Returns the local Translation Matrix
	mat4 GetWorldMatrix() { return worldTransMatrix; } // Returns the world Translation Matrix

	Transform();
	~Transform();
};