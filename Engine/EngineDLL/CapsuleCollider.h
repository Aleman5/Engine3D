#pragma once

#include "Exports.h"
#include "Collider.h"

class BoundingBox;

class ENGINEDLL_API CapsuleCollider : public Collider
{
public:
	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;

	void CreateGeometry(BoundingBox* boundingBox) override;
	void CreateGeometry(float radius, float height);

	CapsuleCollider();
	~CapsuleCollider();
};