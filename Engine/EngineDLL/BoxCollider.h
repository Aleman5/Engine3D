#pragma once

#include "Collider.h"

class BoundingBox;

class ENGINEDLL_API BoxCollider : public Collider
{
public:
	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;

	void CreateGeometry(BoundingBox* boundingBox) override;
	void CreateGeometry(float width, float height, float depth);

	BoxCollider();
	~BoxCollider();
};