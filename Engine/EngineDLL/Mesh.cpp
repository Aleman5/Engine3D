#include "Mesh.h"

Mesh::Mesh(Renderer* renderer, Material* material, const string modelPath, const string texturePath)
	: renderer(renderer), material(material), modelPath(modelPath), sTexturePath(texturePath)
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
	renderer->loadIdentityMatrix();
	renderer->SetModelMatrix(transform->GetModelMatrix());

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
	bool state = ModelImporter::getInstance()->Import3DFromFile(fileName, textureName, m_Entries, m_Textures, renderer);

	for (int i = 0; i < m_Textures.size(); i++)
		bufferTextures.push_back(renderer->GenTexture(m_Textures[i].width, m_Textures[i].height, m_Textures[i].data));

	return state;
}