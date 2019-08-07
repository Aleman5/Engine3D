#pragma once

#include <glm/glm.hpp>
#include "Node.h"
#include "Rigidbody.h"
#include "BoundingBox.h"
#include "Definitions.h"
#include "ModelImporter.h"
#include "Input.h"
#include "ThirdPersonCameraController.h"
#include "BoxCollider.h"

using namespace glm;

enum class RotationDir
{
	PITCH_UPWARDS, PITCH_DOWNWARDS,
	YAW_LEFT, YAW_RIGHT,
	ROLL_LEFT, ROLL_RIGHT
};

class Helicopter
{
private:
	Node* root;
	Node* camera;

	RigidBody* rigidbody;

	float ascensionForce;
	float torqueForce;
	float mass;
	float fuel;

	const float FUEL_DEPLETION_RATE = 10.0f;

	void Ascend();
	void Rotate(RotationDir rotationDir);

public:
	Helicopter();
	~Helicopter();

	void Start(Node* scene, vec3 position, float ascensionForce, float torqueForce, float mass, float fuel);
	void Update();

	PxRigidActor* GetRigidActor() { return rigidbody->GetRigidActor(); };

	inline Node* GetRootObject() const { return root; }
	inline float GetFuel() const { return fuel; }
};
