#include "SimulationEventCallback.h"

SimulationEventCallback::SimulationEventCallback(PxRigidActor* spaceshipActor) : physx::PxSimulationEventCallback(),
spaceshipActor(spaceshipActor)
{

}

SimulationEventCallback::~SimulationEventCallback()
{

}

void SimulationEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs,
	PxU32 nbPairs)
{
	for (int i = 0; i < nbPairs; i++)
	{
		const PxContactPair& cp = pairs[i];

		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if (pairHeader.actors[0] == spaceshipActor || pairHeader.actors[1] == spaceshipActor)
			{
				cout << "Collision detected!" << endl;
			}
		}
	}
}