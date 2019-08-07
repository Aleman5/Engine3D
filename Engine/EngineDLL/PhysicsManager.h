#pragma once

#include <iostream>
#include <glm/vec3.hpp>
#include "PxPhysicsAPI.h"
#include "Definitions.h"
#include "Exports.h"
#include "Enums.h"

using namespace physx;
using namespace glm;

class Renderer;
class Material;

class ENGINEDLL_API PhysicsManager
{
	static PhysicsManager *instance;

	const int VERTEX_COMPONENTS = 3;
	const int LINE_VERTICES = 2;

	PxFoundation* foundation;
	PxPhysics* physics;
	PxCooking* cooking;
	PxScene* scene;

	Material* debugRenderMaterial;
public:
	bool Start(vec3 gravity, unsigned int numThreads);
	void Stop();

	void Simulate();
	void FetchSimulationResults();

	void DrawDebugVisualization() const;

	void AddActor(PxActor* actor);
	void RemoveActor(PxActor* actor);

	void SetCurrentSceneGravity(vec3 gravity);

	void SetSimulationEventCallback(PxSimulationEventCallback* simulationCallback);

	void PrepareMaterial();

	PxMaterial* CreatePhysicsMaterial(float staticFriction, float dynamicFriction, float restitution);
	PxRigidActor* CreateRigidActor(PxTransform pxTransform, bool isStatic);
	PxHeightField* CreateHeighField(PxHeightFieldDesc hfDesc);

	static PhysicsManager* getInstance()
	{
		if (instance == NULL) instance = new PhysicsManager();
		return instance;
	}
	PhysicsManager(PhysicsManager const&) = delete;
	void operator = (PhysicsManager const&) = delete;
private:
	PhysicsManager();
};