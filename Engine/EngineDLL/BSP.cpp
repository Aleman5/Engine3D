#include "BSP.h"
#include "Material.h"
#include "Transform.h"
#include "GlobalDefs.h"

BSP::BSP() : material(Material::GenerateMaterial(TEXTURE_VERTEX_SHADER, TEXTURE_FRAGMENT_SHADER)),
			 renderer(Renderer::getInstance()),
			 position(vec3(100000.0f, 100000.0f, 100000.0f)), fwd(vec3(1.0f, 0.0f, 0.0f))
{
	Start();
}
BSP::~BSP()
{

}

void BSP::Start()
{
	name = "BSP";
	reqTransform = true;

	UpdatePlane();

	renderer->AddBSP(this);
}

void BSP::Update()
{
	if (fwd != transform->GetForward())
	{
		fwd = transform->GetForward();
		UpdatePlane();
	}

	if (position != transform->GetGlobalPosition())
	{
		position = transform->GetGlobalPosition();
		UpdatePlane();
	}

	halfspace = renderer->ClassifyPoint(plane, vec4(renderer->GetCameraGlobalPosition(), 1.0f));
}

void BSP::Draw()
{
	vec3 right = transform->GetRight();
	vec3 up = transform->GetUp();

	vec3 pointRight = position + right * 5.0f;
	vec3 pointUp	= position + up * 5.0f;

	float* vertex = new float[4 * 3]
	{
		 pointRight.x,  pointUp.y, position.z,
		-pointRight.x,  pointUp.y, position.z,
		 pointRight.x, -pointUp.y, position.z,
		-pointRight.x, -pointUp.y, position.z,
	};

	float* verticesColorData = new float[4 * 3]
	{
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};

	vector<unsigned int> indices
	{
		0, 1,
		2, 3,
	};

	if (material != NULL)
	{
		material->Bind();
		material->SetMatrixProperty("MVP", renderer->GetMVP());
		material->BindTexture();
	}

	renderer->EnableAttributes(0);
	renderer->EnableAttributes(1);

	unsigned int id = renderer->GenBuffer(vertex, sizeof(float) * 4 * 3);
	unsigned int colorId = renderer->GenBuffer(verticesColorData, sizeof(float) * 4 * 3);
	unsigned int elementsId = renderer->GenElementBuffer(indices);

	renderer->BindBuffer(id, 0);
	renderer->BindTextureBuffer(colorId, 1);
	renderer->BindElementBuffer(elementsId);
	renderer->DrawElementBuffer(elementsId);

	renderer->DisableAttributes(0);
	renderer->DisableAttributes(1);
}

void BSP::SetTransform(Transform* transform)
{
	this->transform = transform;
}

void BSP::UpdatePlane()
{
	plane = renderer->CreatePlane(fwd, position);
	renderer->NormalizePlane(plane);
}