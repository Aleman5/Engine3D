#pragma once

#include "GL\glew.h"
#include "GLFW\glfw3.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\glm.hpp>

using namespace glm;

#include "Component.h"
#include "Layers.h"

static enum Space
{
	Local,
	World
};

class ENGINEDLL_API Transform : public Component
{
	Layers layer;
	Transform* collision;

	mat4 model;		// Model Matrix

	// Local Matrix
	mat4 lTransMat;	// Translation Matrix
	mat4 lRotX;		// RotationX Matrix
	mat4 lRotY;		// RotationY Matrix
	mat4 lRotZ;		// RotationZ Matrix
	mat4 lScaleMat;	// Scale Matrix

	// World Matrix
	mat4 wTransMat;	// Translation Matrix
	mat4 wRotX;		// RotationX Matrix
	mat4 wRotY;		// RotationY Matrix
	mat4 wRotZ;		// RotationZ Matrix
	mat4 wScaleMat;	// Scale Matrix

public:
	bool isStatic;	// IsStatic Yes/No
	float mass;		// Actual mass

	vec3 vectorPosition; // Actual Position
	vec3 vectorRotation; // Actual Rotation
	vec3 vectorScale;	 // Actual Scale

	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;
	
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

	void SetLayer(Layers layer);

	void CollisionWith(Transform* collision) { this->collision = collision; };
	Transform* OnCollisionEnter() { return collision; };

	Transform* GetTransform() { return this; }		// Returns a pointer to this Transform
	Layers GetLayer() { return layer; }				// Returns the actual layer
	vec3 GetPosition() { return vectorPosition; }	// Returns the actual position
	vec3 GetScale()	{ return vectorScale; }			// Returns the actual scale
	vec3 GetRotation() { return vectorRotation; }	// Returns the actual rotation
	mat4 GetModelMatrix() { return model; }			// Returns the Model Matrix
	mat4 GetLocalMatrix() { return lTransMat; }		// Returns the local Translation Matrix
	mat4 GetWorldMatrix() { return wTransMat; }		// Returns the world Translation Matrix

	Transform();
	~Transform();
};