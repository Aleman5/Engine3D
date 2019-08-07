#include "PhysicsManager.h"
#include "Renderer.h"
#include "Material.h"
#include "GlobalDefs.h"

PhysicsManager* PhysicsManager::instance = NULL;

PxFilterFlags createFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// let triggers through
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlag::eDEFAULT;
}

PhysicsManager::PhysicsManager() : foundation(NULL), physics(NULL), scene(NULL), debugRenderMaterial(NULL)
{

}

bool PhysicsManager::Start(vec3 gravity, unsigned int numThreads)
{
	static PxDefaultAllocator defaultAllocator;
	static PxDefaultErrorCallback defaultErrorCallback;

	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, defaultAllocator, defaultErrorCallback);
	if (!foundation)
	{
		std::cerr << "Failed to initilaize the PxFoundation object." << std::endl;
		return false;
	}

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());
	if (!physics)
	{
		std::cerr << "Failed to initilaize the PxPhysics object." << std::endl;
		return false;
	}

	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(physics->getTolerancesScale()));
	if (!cooking)
	{
		std::cerr << "Failed to initilaize the PxCooking object." << std::endl;
		return false;
	}

	PxSceneDesc sceneDesc(physics->getTolerancesScale());

	PxVec3 physxGravity(gravity.x, gravity.y, gravity.z);
	sceneDesc.gravity = physxGravity;
	sceneDesc.kineKineFilteringMode = PxPairFilteringMode::eKEEP;
	sceneDesc.staticKineFilteringMode = PxPairFilteringMode::eKEEP;

	sceneDesc.filterShader = createFilterShader;

	if (!sceneDesc.cpuDispatcher)
	{
		PxDefaultCpuDispatcher* cpuDispatcher = PxDefaultCpuDispatcherCreate(numThreads);
		sceneDesc.cpuDispatcher = cpuDispatcher;
	}

	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	if (!sceneDesc.isValid())
	{
		std::cerr << "The Scene descriptor is not valid" << std::endl;
		return false;
	}

	scene = physics->createScene(sceneDesc);
	if (!scene)
	{
		std::cerr << "Failed to create the PxScene object" << std::endl;
		return false;
	}

	scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
	scene->setVisualizationParameter(PxVisualizationParameter::eWORLD_AXES, 10.0f);
	scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 5.0f);
	scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

	return true;
}

void PhysicsManager::Stop()
{
	if (debugRenderMaterial)
		Material::DestroyMaterial(debugRenderMaterial);

	if (scene)
		scene->release();
	if (cooking)
		cooking->release();
	if (physics)
		physics->release();
	if (foundation)
		foundation->release();
}

void PhysicsManager::Simulate()
{
	scene->simulate(Defs::getInstance()->deltaTime);
}

void PhysicsManager::FetchSimulationResults()
{
	scene->fetchResults(true);
}

void PhysicsManager::DrawDebugVisualization() const
{
	const PxRenderBuffer& rb = scene->getRenderBuffer();

	Renderer* renderer = Renderer::getInstance();
	renderer->loadIdentityMatrix();

	debugRenderMaterial->Bind();
	debugRenderMaterial->SetMatrixProperty("MVP", renderer->GetMVP());

	for (unsigned int i = 0; i < rb.getNbLines(); i++)
	{
		const PxDebugLine& line = rb.getLines()[i];
		
		float* vertexBufferData = new float[VERTEX_COMPONENTS * LINE_VERTICES]
		{
			line.pos0.x, line.pos0.y, line.pos0.z,
			line.pos1.x, line.pos1.y, line.pos1.z,
		};
		int vertexBufferSize = sizeof(float) * VERTEX_COMPONENTS * LINE_VERTICES;
		unsigned int vertexBufferID = renderer->GenBuffer(vertexBufferData, vertexBufferSize);

		renderer->EnableAttributes(0);
		renderer->BindBuffer(vertexBufferID, 0);
		renderer->DrawBuffer(LINE_VERTICES, PrimitiveType::LINES);
		renderer->DisableAttributes(0);

		renderer->DestroyBuffer(vertexBufferID);
		delete[] vertexBufferData;
	}
}

void PhysicsManager::AddActor(PxActor* actor)
{
	scene->addActor(*actor);
}

void PhysicsManager::RemoveActor(PxActor* actor)
{
	scene->removeActor(*actor);
}

void PhysicsManager::SetCurrentSceneGravity(glm::vec3 gravity)
{
	PxVec3 pxGravity(gravity.x, gravity.y, gravity.z);

	scene->setGravity(pxGravity);
}

void PhysicsManager::SetSimulationEventCallback(PxSimulationEventCallback* simulationCallback)
{
	scene->setSimulationEventCallback(simulationCallback);
}

void PhysicsManager::PrepareMaterial()
{
	debugRenderMaterial = Material::GenerateMaterial(SIMPLE_VERTEX_SHADER, SIMPLE_FRAGMENT_SHADER);
}

PxMaterial* PhysicsManager::CreatePhysicsMaterial(float staticFriction, float dynamicFriction, float restitution)
{
	return physics->createMaterial(staticFriction, dynamicFriction, restitution);
}

PxRigidActor* PhysicsManager::CreateRigidActor(PxTransform pxTransform, bool isStatic)
{
	PxRigidActor* rigidActor = NULL;

	rigidActor = (isStatic) ? (PxRigidActor*)physics->createRigidStatic(pxTransform) :
							  (PxRigidActor*)physics->createRigidDynamic(pxTransform);

	return rigidActor;
}

PxHeightField* PhysicsManager::CreateHeighField(PxHeightFieldDesc hfDesc)
{
	PxHeightField* heightField = NULL;

	heightField = cooking->createHeightField(hfDesc, physics->getPhysicsInsertionCallback());

	return heightField;
}