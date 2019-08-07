#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "RigidBody.h"
#include "Transform.h"
#include "Collider.h"

RigidBody::RigidBody() : rigidActor(NULL), shape(NULL), material(NULL)
{
	Start();
}

RigidBody::~RigidBody()
{

}

void RigidBody::Start()
{
	name = "RigidBody";
	reqTransform = true;
}

void RigidBody::Update()
{
	PxTransform pxTransform = rigidActor->getGlobalPose();
	PxVec3 pxPosition = pxTransform.p;
	PxQuat pxRotation = pxTransform.q;

	vec3 pos = vec3(pxPosition.x, pxPosition.y, pxPosition.z);
	vec4 rotQuat(pxRotation.x, pxRotation.y, pxRotation.z, pxRotation.w);

	transform->SetGlobalPosition(pos.x, pos.y, pos.z);
	transform->ChangeRotationMatrix(rotQuat);
}

void RigidBody::Draw()
{

}

void RigidBody::SetTransform(Transform* transform)
{
	this->transform = transform;
}

void RigidBody::CreateRigidBody(Collider* collider, bool sIsStatic, float mass,
	float staticFriction, float dynamicFriction, float restitution,
	vec3 colliderOffset)
{
	isStatic = sIsStatic;

	vec3 position = transform->GetGlobalPosition();
	vec3 rotation = transform->GetGlobalRotation();

	vec4 rotQuat = Transform::ConvertToQuaternion(rotation.x, rotation.y, rotation.z);

	PxVec3 pxPosition(position.x, position.y, position.z);
	PxQuat pxRotation(rotQuat.x, rotQuat.y, rotQuat.z, rotQuat.w);

	PxVec3 localOffset(colliderOffset.x, colliderOffset.y, colliderOffset.z);
	PxQuat localRot(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f));
	PxTransform pxTransform(pxPosition, pxRotation);
	PxTransform relativePose(localOffset, localRot);

	PhysicsManager* physicsManager = PhysicsManager::getInstance();

	PxGeometry* geometry = collider->GetGeometry();

	rigidActor = physicsManager->CreateRigidActor(pxTransform, isStatic);
	material = physicsManager->CreatePhysicsMaterial(staticFriction, dynamicFriction, restitution);
	shape = PxRigidActorExt::createExclusiveShape(*rigidActor, *geometry, *material);

	shape->setLocalPose(relativePose);

	if (!isStatic)
	{
		PxRigidDynamic* rigidDynamic = (PxRigidDynamic*)rigidActor;
		rigidDynamic->setMassSpaceInertiaTensor(PxVec3(1.0f, 1.0f, 1.0f));
		PxRigidBodyExt::setMassAndUpdateInertia(*rigidDynamic, (PxReal)mass);
	}

	physicsManager->AddActor(rigidActor);
}

void RigidBody::DisposeRigidBody()
{
	if (rigidActor)
	{
		if (material)
			material->release();
		if (shape)
			rigidActor->detachShape(*shape);
		PhysicsManager::getInstance()->RemoveActor(rigidActor);
		rigidActor->release();
	}
}

void RigidBody::AddForce(vec3 force, ForceMode forceMode)
{
	if (isStatic)
	{
		cout << "WARNING: Attempting to add force to a rigid static body." << endl;
		return;
	}

	PxRigidDynamic* rigidDynamic = (PxRigidDynamic*)rigidActor;
	PxVec3 pxForce(force.x, force.y, force.z);

	rigidDynamic->addForce(pxForce, (PxForceMode::Enum)forceMode);
}

void RigidBody::AddTorque(vec3 torque, ForceMode forceMode)
{
	if (isStatic)
	{
		cout << "WARNING: Attempting to add torque to a rigid static body." << endl;
		return;
	}

	PxRigidDynamic* rigidDynamic = (PxRigidDynamic*)rigidActor;
	PxVec3 pxTorque(torque.x, torque.y, torque.z);

	rigidDynamic->addTorque(pxTorque, (PxForceMode::Enum)forceMode);
}

void RigidBody::ClearTorque(ForceMode forceMode)
{
	if (isStatic)
	{
		cout << "WARNING: Attempting to clear force from a rigid static body." << endl;
		return;
	}

	PxRigidDynamic* rigidDynamic = (PxRigidDynamic*)rigidActor;

	rigidDynamic->clearTorque((PxForceMode::Enum)forceMode);
}