#include "Helipad.h"
#include "GlobalDefs.h"
#include "ModelImporter.h"
#include "BoundingBox.h"
#include "BoxCollider.h"
#include "Rigidbody.h"
#include "GamePaths.h"

Helipad::Helipad() : platform(NULL), heightPosition(HELIPAD_DEFAULT_Y_POS)
{
}
Helipad::~Helipad()
{
}

void Helipad::Start(Node* scene, float sHeightPosition, vec2 minHorPos, vec2 maxHorPos)
{
	platform = new Node("Platform", scene);
	ModelImporter::getInstance()->Load(platform, HELIPAD_PATH, HELIPAD_TEXTURE_PATH);
	cout << "me llamaron xdxd" << endl;
	heightPosition = sHeightPosition;

	platform->transform->Scale(5.0f, 5.0f, 5.0f);
	SetRandomPosition(minHorPos, maxHorPos);

	BoundingBox* bb = new BoundingBox();
	for (int i = 0; i < CUBE_VERTICES; i++) {
		vec3 v = platform->fcData.vertex[i];
		bb->SetVertice(i, v);
	}
	platform->AddComponent(bb);

	BoxCollider* bc = (BoxCollider*)platform->AddComponent(new BoxCollider());
	RigidBody*   rb = (RigidBody*)  platform->AddComponent(new RigidBody());

	bc->CreateGeometry(bb);
	rb->CreateRigidBody(bc, true, 500.0f, 0.5f, 0.5f, 0.0f, vec3(0.0f, 18.0f, 0.0f));
}

void Helipad::SetRandomPosition(vec2 min, vec2 max)
{
	srand(time(0));

	float posX = rand() % (int)(max.x - min.x) + min.x;
	float posZ = rand() % (int)(max.y - min.y) + min.y;

	platform->transform->SetGlobalPosition(posX, heightPosition, posZ);
}