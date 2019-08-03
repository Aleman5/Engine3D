#pragma once

#include <iostream>
#include <glm/vec3.hpp>
#include "Exports.h"
#include "Component.h"

#define CUBE_VERTICES 8

using namespace glm;
using namespace std;

class Transform;

class ENGINEDLL_API BoundingBox : public Component
{
	Transform* transform;
	vec3 vertices[CUBE_VERTICES];

	vec3 maxs;
	vec3 mins;

public:
	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;

	void SetVertices(vec3 vertices[CUBE_VERTICES]);
	void SetVertices(vec4 vertices[CUBE_VERTICES]);

	vec3 GetVertexGlobalPosition(unsigned int index) const;

	inline vec3 GetMaxs() const { return maxs; }
	inline vec3 GetMins() const { return mins; }

	BoundingBox();
	~BoundingBox();
};