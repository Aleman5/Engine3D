#pragma once

#include <time.h>
#include <stdlib.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Node.h"

using namespace glm;

class Helipad
{
	const float HELIPAD_DEFAULT_Y_POS = 100.0f;

	Node* platform;

	float heightPosition;

public:
	void Start(Node* scene, float heightPosition, vec2 minHorPos, vec2 maxHorPos);
	void SetRandomPosition(vec2 min, vec2 max);

	inline Node* GetPlatform() const { return platform; }

	Helipad();
	~Helipad();
};

