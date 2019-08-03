#pragma once

#include "PxPhysicsAPI.h"
#include "Exports.h"
#include "Component.h"
#include "Transform.h"
#include "BoundingBox.h"

using namespace physx;

class BoundingBox;

class ENGINEDLL_API Collider : public Component
{
protected:
	PxGeometry* geometry;
	Transform* transform;

public:
	Collider();
	virtual ~Collider();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void SetTransform(Transform* transform) override;

	virtual void CreateGeometry(BoundingBox* boundingBox) = 0;

	inline PxGeometry* GetGeometry() { return geometry; }
};

