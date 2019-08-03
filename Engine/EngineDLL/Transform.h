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
	const float FULL_ROTATION = 360.0f;

	Layers layer;
	Transform* parent;
	Transform* collision;

	mat4 model;		// Model Matrix

	// Local Matrix
	mat4 lTransMat;	// Translation Matrix
	mat4 lRot;		// Rotation Matrix
	mat4 lScaleMat;	// Scale Matrix

	vec3 right;
	vec3 up;
	vec3 fwd;
	
	vec3 lRight;
	vec3 lUp;
	vec3 lFwd;

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
	void Rotate(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	void ChangeRotationMatrix(vec4 quaternion);

	void ClampEulerRotation();
	void UpdateDirectionVectors();
	void UpdateRotationMatrix();
	void ConvertToEulerAngles(const vec4& quaternion, float& pitch, float& yaw, float& roll);

	void SetLayer(Layers layer);
	void SetParent(Transform* parent);
	void SetGlobalPosition(float x, float y, float z);
	void SetGlobalRotation(float x, float y, float z);

	static vec4 ConvertToQuaternion(float pitch, float yaw, float roll);

	void CollisionWith(Transform* collision) { this->collision = collision; };
	Transform* OnCollisionEnter() { return collision; };

	Transform* GetTransform() { return this;		} // Returns a pointer to this Transform
	Transform* GetParent(){ return parent;			} // Returns the actual parent
	Layers GetLayer()	  { return layer;			} // Returns the actual layer
	vec3 GetPosition()	  { return vectorPosition;	} // Returns the actual position
	vec3 GetScale()	 	  { return vectorScale;		} // Returns the actual scale
	vec3 GetRotation()	  { return vectorRotation;	} // Returns the actual rotation
	mat4 GetModelMatrix() { return model;			} // Returns the Model Matrix
	mat4 GetLocalMatrix() { return lTransMat;		} // Returns the local Translation Matrix

	inline vec3 GetForward() const { return fwd;   }
	inline vec3 GetRight() const   { return right; }
	inline vec3 GetUp() const	   { return up;	   }

	inline vec3 GetLocalForward() const { return lFwd;	 }
	inline vec3 GetLocalRight() const	{ return lRight; }
	inline vec3 GetLocalUp() const		{ return lUp;	 }

	vec3 GetGlobalPosition(); // Returns the Global Position
	vec3 GetGlobalRotation(); // Returns the Global Rotation

	Transform();
	~Transform();
};