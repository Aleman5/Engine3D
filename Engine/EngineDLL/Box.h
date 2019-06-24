#pragma once

#include "Component.h"
#include "Transform.h"

class ENGINEDLL_API Box : Component
{
public:
	vec3 size;
	Transform* transform;

	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;

	Transform* GetTransform() { return transform; };

	Box();
	~Box();
};

