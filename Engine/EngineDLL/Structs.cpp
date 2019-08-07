#include "Structs.h"

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

void MeshEntry::Init(const vector<Vertex>& Vertices, const vector<unsigned int>& Indices)
{
	indices = Indices;
	NumIndices = Indices.size();

	float* positions = new float[Vertices.size() * 3];
	float* textures = new float[Vertices.size() * 2];
	float* normals = new float[Vertices.size() * 3];

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

	Renderer* renderer = Renderer::getInstance();

	//verticesBuffer = renderer->GenVertexBuffer(Vertices);
	verticesBuffer = renderer->GenBuffer(positions, sizeof(float) * Vertices.size() * 3);
	indicesBuffer = renderer->GenElementBuffer(Indices);
	uvBuffer = renderer->GenBuffer(textures, sizeof(float) * Vertices.size() * 2);

	delete positions;
	delete textures;
	delete normals;
}