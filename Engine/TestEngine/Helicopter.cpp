#include "Helicopter.h"
#include "SimulationEventCallback.h"
#include "GlobalDefs.h"
#include "GamePaths.h"
#include "Constants.h"

Helicopter::Helicopter() :
	root(NULL), camera(NULL), rigidbody(NULL),
	ascensionForce(HELI_ASCENSION_FORCE), torqueForce(HELI_TORQUE_FORCE), mass(HELI_MASS), fuel(HELI_INITIAL_FUEL)
{
}

Helicopter::~Helicopter()
{
}

void Helicopter::Ascend()
{
	if (fuel > 0.0f)
	{
		vec3 force = root->transform->GetUp() * ascensionForce;
		rigidbody->AddForce(force, ForceMode::FORCE);

		fuel = max(fuel - FUEL_DEPLETION_RATE * (float)Defs::getInstance()->deltaTime, 0.0f);
	}
}

void Helicopter::Rotate(RotationDir rotationDir)
{
	vec3 torque(0.0f);

	switch (rotationDir)
	{
	case RotationDir::PITCH_UPWARDS:
		torque = root->transform->GetRight() * -torqueForce;
		break;

	case RotationDir::PITCH_DOWNWARDS:
		torque = root->transform->GetRight() * torqueForce;
		break;

	case RotationDir::YAW_LEFT:
		torque = root->transform->GetUp() * torqueForce;
		break;

	case RotationDir::YAW_RIGHT:
		torque = root->transform->GetUp() * -torqueForce;
		break;

	case RotationDir::ROLL_LEFT:
		torque = root->transform->GetForward() * -torqueForce;
		break;

	case RotationDir::ROLL_RIGHT:
		torque = root->transform->GetForward() * torqueForce;
		break;
	}

	rigidbody->AddTorque(torque, ForceMode::FORCE);
}

void Helicopter::Start(Node* scene, vec3 position, float sAscensionForce, float sTorqueForce, float sMass, float sFuel)
{
	root = new Node("Heli", scene);
	camera = new Node("HeliCamera", root);

	ModelImporter::getInstance()->Load(root, SPACESHIP_PATH, SPACESHIP_TEXTURE_PATH);

	ascensionForce = sAscensionForce;
	torqueForce = sTorqueForce;
	mass = sMass;
	fuel = sFuel;

	root->transform->Teleport(position.x, position.y, position.z);

	BoundingBox* bb = new BoundingBox();
	for (int i = 0; i < CUBE_VERTICES; i++) {
		vec3 v = root->fcData.vertex[i];
		bb->SetVertice(i, v);
	}
	root->AddComponent(bb);

	Material* mat = Material::GenerateMaterial(CEL_SHADING_VERTEX_SHADER, CEL_SHADING_FRAGMENT_SHADER);

	Camera* c = (Camera*)camera->AddComponent(new Camera());
	c->SetAsMainCamera();
	ThirdPersonCameraController* tpcc = (ThirdPersonCameraController*)camera->AddComponent(new ThirdPersonCameraController());
	BoxCollider* bc = (BoxCollider*)root->AddComponent(new BoxCollider());

	rigidbody = (RigidBody*)root->AddComponent(new RigidBody());

	bc->CreateGeometry(bb);
	rigidbody->CreateRigidBody(bc, false, mass, 1.25f, 1.25f, 1.25f);

	tpcc->SetUpController(camera, root, 70.0f, 120.0f);
}

void Helicopter::Update()
{
	Input* input = Input::getInstance();
	
	if (input->isInput(GLFW_KEY_SPACE))
		Ascend();

	if (input->isInput(GLFW_KEY_W))
		Rotate(RotationDir::PITCH_UPWARDS);

	if (input->isInput(GLFW_KEY_S))
		Rotate(RotationDir::PITCH_DOWNWARDS);

	if (input->isInput(GLFW_KEY_Q))
		Rotate(RotationDir::YAW_LEFT);

	if (input->isInput(GLFW_KEY_E))
		Rotate(RotationDir::YAW_RIGHT);

	if (input->isInput(GLFW_KEY_A))
		Rotate(RotationDir::ROLL_LEFT);

	if (input->isInput(GLFW_KEY_D))
		Rotate(RotationDir::ROLL_RIGHT);
}