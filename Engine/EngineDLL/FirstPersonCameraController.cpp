#include "FirstPersonCameraController.h"
#include "Input.h"

FirstPersonCameraController::FirstPersonCameraController()
{
	Start();
}
FirstPersonCameraController::~FirstPersonCameraController()
{
}

void FirstPersonCameraController::Start()
{
	name = "ThirdPersonCameraController";
	reqTransform = false;
}
void FirstPersonCameraController::Update()
{
	Input* input = Input::getInstance();

	float horRotation = input->GetAxis(Axis::HORIZONTAL) * rotationSpeed * Defs::getInstance()->deltaTime;
	float verRotation = input->GetAxis(Axis::VERTICAL)   * rotationSpeed * Defs::getInstance()->deltaTime;

	Rotate(horRotation, verRotation);
}
void FirstPersonCameraController::Draw()
{

}
void FirstPersonCameraController::SetTransform(Transform* transform)
{

}

void FirstPersonCameraController::Rotate(float horRotation, float verRotation)
{
	vec3 newCameraPosition = cameraTransform->GetPosition();

	horAngle += horRotation;
	verAngle += verRotation;

	verAngle = clamp(verAngle, -VERTICAL_RANGE, VERTICAL_RANGE);

	newCameraPosition.x = initialPivotRight.x	* cos(radians(horAngle));
	newCameraPosition.y = initialPivotUp.y		* sin(radians(verAngle));
	newCameraPosition.z = initialPivotForward.z * sin(radians(horAngle));

	vec3 diff = followTargetTransform->GetGlobalPosition() - cameraTransform->GetGlobalPosition();

	cameraTransform->Teleport(newCameraPosition.x, newCameraPosition.y, newCameraPosition.z);
	camera->SetViewDirection(diff);

	verRotation = clamp(verRotation, -VERTICAL_RANGE, VERTICAL_RANGE);

	camera->Pitch(verRotation);
	camera->Yaw(horRotation);

	//camera-> up = vec4(0, 1, 0, 0);
}

void FirstPersonCameraController::SetUpController(Node* sCameraNode, Node* sFollowTargetNode,
	float sRotationSpeed)
{
	cameraTransform = sCameraNode->transform;
	followTargetTransform = sFollowTargetNode->transform;

	camera = (Camera*)sCameraNode->GetComponent("Camera");

	rotationSpeed = sRotationSpeed;

	initialPivotForward = followTargetTransform->GetForward();
	initialPivotUp = followTargetTransform->GetUp();
	initialPivotRight = followTargetTransform->GetRight();

	cameraTransform->SetRotation(0.0f, 0.0f, 0.0f);
	cameraTransform->Teleport(0.0f, 0.0f, 0.0f);
}