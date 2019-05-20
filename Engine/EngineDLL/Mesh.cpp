#include "Mesh.h"

MeshEntry::MeshEntry()
{
	verticesBuffer = NULL;
	indicesBuffer = NULL;
	NumIndices = 0;
	MaterialIndex = NULL;
};

MeshEntry::~MeshEntry()
{
	if (verticesBuffer != NULL) glDeleteBuffers(1, &verticesBuffer);
	if (indicesBuffer != NULL) glDeleteBuffers(1, &indicesBuffer);
}

void MeshEntry::Init(const vector<Vertex>& Vertices, const vector<unsigned int>& Indices, Renderer* renderer)
{
	NumIndices = Indices.size();

	float* positions = new float[Vertices.size() * 3];
	float* textures  = new float[Vertices.size() * 2];
	float* normals   = new float[Vertices.size() * 3];

	for (size_t i = 0; i < Vertices.size(); i++)
	{
		positions[i * 3] = Vertices[i].m_pos.x;
		positions[i * 3 + 1] = Vertices[i].m_pos.y;
		positions[i * 3 + 2] = Vertices[i].m_pos.z;
		textures[i * 2] = Vertices[i].m_tex.x;
		textures[i * 2 + 1] = Vertices[i].m_tex.y;
		normals[i * 3] = Vertices[i].m_normal.x;
		normals[i * 3 + 1] = Vertices[i].m_normal.y;
		normals[i * 3 + 2] = Vertices[i].m_normal.z;
	}

	//verticesBuffer = renderer->GenVertexBuffer(Vertices);
	verticesBuffer = renderer->GenBuffer(positions, sizeof(float) * Vertices.size() * 3);
	indicesBuffer = renderer->GenElementBuffer(Indices);
	uvBuffer = renderer->GenBuffer(textures, sizeof(float) * Vertices.size() * 2);
}

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