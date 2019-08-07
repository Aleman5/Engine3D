#include "ThirdPersonCameraController.h"
#include "Input.h"

ThirdPersonCameraController::ThirdPersonCameraController()
{
	Start();
}
ThirdPersonCameraController::~ThirdPersonCameraController()
{
}

void ThirdPersonCameraController::Start()
{
	name = "ThirdPersonCameraController";
	reqTransform = false;
}
void ThirdPersonCameraController::Update()
{
	Input* input = Input::getInstance();

	float horRotation = input->GetAxis(Axis::HORIZONTAL) * rotationSpeed * Defs::getInstance()->deltaTime;
	float verRotation = input->GetAxis(Axis::VERTICAL)   * rotationSpeed * Defs::getInstance()->deltaTime;

	Rotate(horRotation, verRotation);
}
void ThirdPersonCameraController::Draw()
{

}
void ThirdPersonCameraController::SetTransform(Transform* transform)
{
	
}

void ThirdPersonCameraController::Rotate(float horRotation, float verRotation)
{
	vec3 newCameraPosition = cameraTransform->GetPosition();

	horAngle += horRotation;
	verAngle += verRotation;

	verAngle = clamp(verAngle, -VERTICAL_RANGE, VERTICAL_RANGE);

	newCameraPosition.x = initialPivotRight.x * cos(radians(horAngle)) * radius;
	newCameraPosition.y = initialPivotUp.y * sin(radians(verAngle)) * radius;
	newCameraPosition.z = initialPivotForward.z * sin(radians(horAngle)) * radius;

	vec3 diff = followTargetTransform->GetGlobalPosition() - cameraTransform->GetGlobalPosition();

	cameraTransform->Teleport(newCameraPosition.x, newCameraPosition.y, newCameraPosition.z);
	camera->SetViewDirection(diff);
}

void ThirdPersonCameraController::SetUpController(Node* sCameraNode, Node* sFollowTargetNode,
	float rotationSpeed, float radius)
{
	cameraTransform = sCameraNode->transform;
	followTargetTransform = sFollowTargetNode->transform;

	camera = (Camera*)sCameraNode->GetComponent("Camera");

	rotationSpeed = rotationSpeed;
	radius = radius;

	initialPivotForward = followTargetTransform->GetForward();
	initialPivotUp = followTargetTransform->GetUp();
	initialPivotRight = followTargetTransform->GetRight();

	cameraTransform->SetRotation(0.0f, 0.0f, 0.0f);
	cameraTransform->Teleport(0.0f, 0.0f, -radius);
}