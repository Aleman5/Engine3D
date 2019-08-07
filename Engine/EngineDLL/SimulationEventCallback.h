#pragma once

#include <iostream>
#include <PxPhysicsAPI.h>
#include "Exports.h"

using namespace physx;
using namespace std;

class ENGINEDLL_API SimulationEventCallback : public PxSimulationEventCallback
{
private:
	PxRigidActor* spaceshipActor;
	PxRigidActor* helipadActor;

public:
	SimulationEventCallback();
	~SimulationEventCallback();

	void AddSpaceshipRigidActor(PxRigidActor* sSpaceshipActor) { spaceshipActor = sSpaceshipActor; };
	void AddHelipadRigidActor(PxRigidActor* sHelipadActor) { helipadActor = sHelipadActor; };

	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs,
		PxU32 nbPairs) override;
	void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) override {}
	void onWake(PxActor** actors, PxU32 count) override {}
	void onSleep(PxActor** actors, PxU32 count) override {}
	void onTrigger(PxTriggerPair* pairs, PxU32 count) override {}
	void onAdvance(const PxRigidBody *const *bodyBuffer, const PxTransform *poseBuffer,
		const PxU32 count) override {}
};