#include "Mesh.h"

Mesh::Mesh(Material* material, const string modelPath, const string texturePath)
	: material(material), modelPath(modelPath), sTexturePath(texturePath)
{
	Start();
}
Mesh::~Mesh()
{
	if (texturePath) delete texturePath;
}

void Mesh::Start()
{
	name = "Mesh";
	reqTransform = true;

	renderer = Renderer::getInstance();

	srand(time(0));

	LoadMesh(modelPath, sTexturePath);

	this->texturePath = new char[sTexturePath.size() + 1];
	sTexturePath.copy(this->texturePath, sTexturePath.size() + 1);
	this->texturePath[sTexturePath.size()] = '\0';
}

void Mesh::Update()
{
	
}

void Mesh::Draw()
{
	Plane* planes = renderer->GetPlanes();

	for (int i = 0; i < 6; i++)
	{
		bool allBehind = true;

		for (int j = 0; j < 8; j++)
		{
			if (renderer->ClassifyPoint(planes[i], fcData.vertex[j] * transform->GetModelMatrix()) == POSITIVE)
			{
				allBehind = false;
				break;
			}
			if (allBehind) return;
		}
	}

	if (material != NULL)
	{
		material->Bind("myTextureSampler", bufferTextures[0]);
		material->SetMatrixProperty("MVP", renderer->GetMVP());
	} 

	renderer->EnableAttributes(0);
	renderer->EnableAttributes(1);

	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		renderer->BindBuffer(m_Entries[i].verticesBuffer, 0);
		renderer->BindTextureBuffer(m_Entries[i].uvBuffer, 1);
		renderer->BindElementBuffer(m_Entries[i].indicesBuffer);
		renderer->DrawElementBuffer(m_Entries[i].NumIndices);
	}

	renderer->DisableAttributes(0);
	renderer->DisableAttributes(1);
}

void Mesh::SetTransform(Transform* transform)
{
	this->transform = transform;
}

bool Mesh::LoadMesh(const string& fileName, const string& textureName)
{
	bool state = ModelImporter::getInstance()->Import3DFromFile(fileName, textureName, m_Entries, m_Textures, fcData, renderer);

	for (int i = 0; i < m_Textures.size(); i++)
		bufferTextures.push_back(renderer->GenTexture(m_Textures[i].width, m_Textures[i].height, m_Textures[i].imageFormat, m_Textures[i].data));

	return state;
}