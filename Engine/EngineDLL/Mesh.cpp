#include "Mesh.h"

Mesh::Mesh() : material(Material::GenerateMaterial(CEL_SHADING_VERTEX_SHADER, CEL_SHADING_FRAGMENT_SHADER)),
			   debugMaterial(Material::GenerateMaterial(TEXTURE_VERTEX_SHADER, TEXTURE_FRAGMENT_SHADER))
{
	Start();
}
Mesh::~Mesh()
{
	delete material;
}

void Mesh::Start()
{
	name = "Mesh";
	reqTransform = true;

	renderer = Renderer::getInstance();

	debugMode = false;
}

void Mesh::Update()
{
	
}

void Mesh::Draw()
{
	if (material != NULL)
	{
		material->Bind();
		material->SetMatrixProperty("MVP", renderer->GetMVP());
		material->SetVec2Property  ("iResolution", vec2(renderer->GetWindowWidht(), renderer->GetWindowHeight()));
		material->SetVec3Property  ("iLightDir", Defs::getInstance()->lightDirection);
		material->SetVec3Property  ("iCameraFwd", renderer->GetCameraFwd());
		material->SetFloatProperty ("iTime", Defs::getInstance()->currentTime);
		material->SetFloatProperty ("iFov", renderer->GetFov());
		material->SetFloatProperty ("iAngleLimit", renderer->GetAngleLimit());
		material->BindTexture();
	} 

	renderer->EnableAttributes(0);
	renderer->EnableAttributes(1);
	renderer->EnableAttributes(2);

	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		renderer->BindBuffer(m_Entries[i].verticesBuffer, 0);
		renderer->BindTextureBuffer(m_Entries[i].uvBuffer, 1);
		renderer->BindNormalBuffer (m_Entries[i].normalBuffer, 2);
		renderer->BindElementBuffer(m_Entries[i].indicesBuffer);
		renderer->DrawElementBuffer(m_Entries[i].NumIndices);
	}

	renderer->DisableAttributes(0);
	renderer->DisableAttributes(1);
	renderer->DisableAttributes(2);
}

void Mesh::DrawFCData(const FCCubeData& fcData)
{
	float* fcVertex = new float[12*3]
	{
		fcData.vertex[0].x, fcData.vertex[0].y, fcData.vertex[0].z,
		fcData.vertex[1].x, fcData.vertex[1].y, fcData.vertex[1].z,
		fcData.vertex[4].x, fcData.vertex[4].y, fcData.vertex[4].z,
		
		fcData.vertex[1].x, fcData.vertex[1].y, fcData.vertex[1].z,
		fcData.vertex[4].x, fcData.vertex[4].y, fcData.vertex[4].z,
		fcData.vertex[5].x, fcData.vertex[5].y, fcData.vertex[5].z,
		
		fcData.vertex[2].x, fcData.vertex[2].y, fcData.vertex[2].z,
		fcData.vertex[3].x, fcData.vertex[3].y, fcData.vertex[3].z,
		fcData.vertex[6].x, fcData.vertex[6].y, fcData.vertex[6].z,
		
		fcData.vertex[3].x, fcData.vertex[3].y, fcData.vertex[3].z,
		fcData.vertex[6].x, fcData.vertex[6].y, fcData.vertex[6].z,
		fcData.vertex[7].x, fcData.vertex[7].y, fcData.vertex[7].z,
	};

	float* verticesColorData = new float[3*12]
	{
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};

	vector<unsigned int> indices{
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9,10,11
	};

	if (debugMaterial != NULL)
	{
		debugMaterial->Bind();
		debugMaterial->SetMatrixProperty("MVP", renderer->GetMVP());
		debugMaterial->BindTexture();
	}

	renderer->EnableAttributes(0);
	renderer->EnableAttributes(1);

	unsigned int id = renderer->GenBuffer(fcVertex, sizeof(float) * 12 * 3);
	unsigned int colorId = renderer->GenBuffer(verticesColorData, sizeof(float) * 12 * 3);
	unsigned int elementsId = renderer->GenElementBuffer(indices);

	renderer->BindBuffer(id, 0);
	renderer->BindTextureBuffer(colorId, 1);
	renderer->BindElementBuffer(elementsId);
	renderer->DrawElementBuffer(elementsId);

	renderer->DisableAttributes(0);
	renderer->DisableAttributes(1);
}

void Mesh::SetTransform(Transform* transform)
{
	this->transform = transform;
}

void Mesh::SetDiffTex(vector<Header> diffuseTextures)
{
	if (diffuseTextures.size() > 0)
	{
		material = Material::GenerateMaterial(CEL_SHADING_VERTEX_SHADER, CEL_SHADING_FRAGMENT_SHADER);
		m_Textures = diffuseTextures;
		material->SetTexture(diffuseTextures[0], "myTextureSampler");
	}
	else
		material = Material::GenerateMaterial(SIMPLE_VERTEX_SHADER, SIMPLE_FRAGMENT_SHADER);
}

void Mesh::ActivateDebugMode()
{
	debugMode = true;
}

void Mesh::DesactivateDebugMode()
{
	debugMode = false;
}

bool Mesh::GetDebugMode()
{
	return debugMode;
}