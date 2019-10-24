#pragma once

#include "Exports.h"
#include "Renderer.h"
#include "Component.h"

class Transform;

class ENGINEDLL_API BSP : public Component
{
	Transform* transform;
	Renderer* renderer;

	vec3 position;
	vec3 direction;

public:
	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;

	vec4 plane;
	Halfspace halfspace;

	BSP(const vec3& direction);
	~BSP();
};