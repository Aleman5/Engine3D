#include "Mesh.h"

Mesh::Mesh(Renderer* renderer, Material* material, Layers tag, const string modelPath, string texturePath) : Entity(renderer, material, tag)
{
	srand(time(0));

	LoadMesh(modelPath, texturePath);

	this->texturePath = new char[texturePath.size() + 1];
	texturePath.copy(this->texturePath, texturePath.size() + 1);
	this->texturePath[texturePath.size()] = '\0';
}
Mesh::~Mesh()
{
	if (texturePath) delete texturePath;
}

void Mesh::Update()
{
	Entity::Update();
}

bool Mesh::LoadMesh(const string& fileName, const string& textureName)
{
	bool state = ModelImporter::getInstance()->Import3DFromFile(fileName, textureName, m_Entries, m_Textures, renderer);

	for (int i = 0; i < m_Textures.size(); i++)
		bufferTextures.push_back(renderer->GenTexture(m_Textures[i].width, m_Textures[i].height, m_Textures[i].data));

	return state;
}

void Mesh::ShouldDispose()
{
	if (shouldDispose)
	{
		renderer->DestroyBuffer(bufferId);
		shouldDispose = false;
	}
}

unsigned int Mesh::SetVertices(float* vertices, int count)
{
	return 0;
}

void Mesh::Draw()
{
	renderer->loadIdentityMatrix();
	renderer->SetModelMatrix(model);

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