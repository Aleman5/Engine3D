#pragma once

#include "Renderer.h"
#include "Material.h"
#include "Layers.h"

class ENGINEDLL_API Entity
{
protected:
	GLenum drawMode;			// Mode of the Draw
	Renderer* renderer;			// Renderer reference
	Material* material;			// Material reference
	unsigned int bufferId;		// Id of the Buffer
	unsigned int colorBufferId;	// Id of the ColorBuffer
	unsigned int programId;		// Id of the Program
	bool shouldDispose;			// Should Dispose? Yes/No

	vec3 vectorPosition;	// Actual Position
	vec3 vectorRotation;	// Actual Rotation
	vec3 vectorScale;		// Actual Scale

	mat4 model;			// Model Matrix

	mat4 translationMatrix;// Translation Matrix
	mat4 rotateX;			// RotationX Matrix
	mat4 rotateY;			// RotationX Matrix
	mat4 rotateZ;			// RotationX Matrix
	mat4 scallingMatrix;	// Scale Matrix

	// Box Collider things
	vec2 col;	// Box Collider proportions
	float mass;		// "Mass"
	bool isStatic;	// Is static? Yes/No
	Layers tag;		// Tag

	Entity* collision;

	void Update();

public:
	virtual void Draw() = 0;
	virtual void ShouldDispose() = 0;

	void UpdateModel();
	void Translate(
		glm::vec3 vector3	// Vector3 to Translate
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
		glm::vec3 vector3	// Vector3 to Scale
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
	virtual unsigned int SetVertices(
		float* vertices,	// Vertices data
		int count			// Total of vertices
	) = 0;

	void CollisionWith(Entity* collision) { this->collision = collision; };
	Entity* OnCollisionEnter() { return collision; };

	void SetMass(float newMass)		{ mass = newMass;			 } // Set the new value mass
	void SetIsStatic(bool isStatic) { this->isStatic = isStatic; } // Set the new value mass

	Entity* GetEntity()     { return this;			 } // Returns a pointer to this Entity
	Renderer* GetRenderer() { return renderer;		 } // Returns a pointer to the Renderer
	Layers GetTag()			{ return tag;			 } // Returns the tag
	vec3 GetPosition()		{ return vectorPosition; } // Returns the actual position
	vec3 GetScale()			{ return vectorScale;	 } // Returns the actual scale
	vec3 GetRotation()		{ return vectorRotation; } // Returns the actual rotation
	vec2 GetColProps()		{ return col;			 } // Returns the collision proportions
	float GetMass()			{ return mass;			 } // Returns the mass
	bool IsStatic()			{ return isStatic;		 } // Is static? Yes/No

	Entity(Renderer* renderer,	// Renderer reference
		   Material* material,  // Material reference
		   Layers tag			// Tag of the Entity
	);
	~Entity();
};