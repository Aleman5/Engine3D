#pragma once

#include "Exports.h"
#include "Renderer.h"
#include "Component.h"

class Material;
class Transform;

class ENGINEDLL_API BSP : public Component
{
	Transform* transform;
	Material* material;
	Renderer* renderer;

	vec3 position;
	vec3 fwd;

	void UpdatePlane();

public:
	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;

	vec4 plane;
	Halfspace halfspace;

	BSP();
	~BSP();
};