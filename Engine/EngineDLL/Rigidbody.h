#pragma once

#include <iostream>
#include <PxPhysicsAPI.h>
#include "Exports.h"
#include "Component.h"
#include "Enums.h"
#include "PhysicsManager.h"

using namespace physx;
using namespace glm;
using namespace std;

class Transform;
class Collider;

class ENGINEDLL_API RigidBody : public Component
{
private:
	Transform* transform;

	PxRigidActor* rigidActor;
	PxShape* shape;
	PxMaterial* material;

	bool isStatic;

public:
	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;

	void CreateRigidBody(Collider* collider, bool isStatic = false, float mass = 1.0f,
		float staticFriction = 0.1f, float dynamicFriction = 0.1f, float restitution = 0.1f,
		vec3 colliderOffset = vec3(0.0f, 0.0f, 0.0f));
	void DisposeRigidBody();

	void AddForce(vec3 force, ForceMode forceMode);
	void AddTorque(vec3 torque, ForceMode forceMode);
	void ClearTorque(ForceMode forceMode);

	inline PxRigidActor* GetRigidActor() const { return rigidActor; }

	RigidBody();
	virtual ~RigidBody();
};