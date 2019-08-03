#pragma once

#include <glm/vec3.hpp>
#include "Exports.h"
#include "Component.h"
#include "Node.h"
#include "Transform.h"
#include "Camera.h"
#include "Definitions.h"

using namespace glm;

class ENGINEDLL_API ThirdPersonCameraController : public Component
{
	Transform* cameraTransform;
	Transform* followTargetTransform;

	Camera* camera;

	vec3 initialPivotForward;
	vec3 initialPivotUp;
	vec3 initialPivotRight;

	float rotationSpeed;
	float horAngle;
	float verAngle;
	float radius;

	const float VERTICAL_RANGE = 70.0f;

	void Rotate(float horRotation, float verRotation);

public:
	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;

	void SetUpController(Node* cameraNode, Node* followTargetNode, float rotationSpeed, float radius);

	ThirdPersonCameraController();
	~ThirdPersonCameraController();
};