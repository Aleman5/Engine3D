#pragma once

#include <time.h>
#include <stdlib.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Node.h"
#include "Rigidbody.h"

using namespace glm;

class Helipad
{
	const float HELIPAD_DEFAULT_Y_POS = 100.0f;

	Node* platform;
	RigidBody* rigidbody;

	float heightPosition;

public:
	void Start(Node* scene, float heightPosition, vec2 minHorPos, vec2 maxHorPos);
	void SetRandomPosition(vec2 min, vec2 max);

	PxRigidActor* GetRigidActor() { return rigidbody->GetRigidActor(); };

	inline Node* GetPlatform() const { return platform; }

	Helipad();
	~Helipad();
};

