#pragma once

#include "Exports.h"
#include "Renderer.h"
#include "Component.h"

class ENGINEDLL_API BSP : public Component
{
public:
	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;

	vec4 plane;
	Halfspace actualHalfspace;

	BSP(const vec4& plane);
	~BSP();
};