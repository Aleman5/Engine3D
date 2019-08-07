#include "SimulationEventCallback.h"

SimulationEventCallback::SimulationEventCallback() : physx::PxSimulationEventCallback(),
spaceshipActor(NULL), helipadActor(NULL)
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
			if ((pairHeader.actors[0] == spaceshipActor && pairHeader.actors[1] == helipadActor) ||
				(pairHeader.actors[0] == helipadActor && pairHeader.actors[1] == spaceshipActor))
			{
				cout << "Ganaste profe! :)" << endl;
				return;
			}

			if (pairHeader.actors[0] == spaceshipActor || pairHeader.actors[1] == spaceshipActor)
			{
				cout << "Perdiste :(" << endl;
			}
		}
	}
}