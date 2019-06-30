#pragma once

#include "Exports.h"
#include "Renderer.h"
#include "GL\glew.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL

#include "GLFW\glfw3.h"

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices)
#define ASSIMP_LOAD_FLAGS_TRIANG_FLIP (aiProcess_Triangulate | aiProcess_FlipUVs)

using namespace std;

struct Vertex
{
	glm::vec3 m_pos;
	glm::vec2 m_tex;
	glm::vec3 m_normal;

	Vertex() {}

	Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};

struct MeshEntry
{
	void Init(
		const std::vector<Vertex>& Vertices,
		const std::vector<unsigned int>& Indices,
		Renderer* renderer
	);

	unsigned int verticesBuffer;
	unsigned int indicesBuffer;
	unsigned int uvBuffer;


	unsigned int NumIndices;
	unsigned int MaterialIndex;

	MeshEntry();
	~MeshEntry();
};