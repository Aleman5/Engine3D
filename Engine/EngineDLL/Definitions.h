#pragma once

#include "Exports.h"
#include "GL\glew.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL

#include "GLFW\glfw3.h"

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

class Renderer;

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

	unsigned int NumIndices;
	unsigned int MaterialIndex;

	MeshEntry();
	~MeshEntry();
};

class ENGINEDLL_API Defs
{
	static Defs *instance;
	double currentFrame; // Actual Time
	double lastFrame;	// Previous frame

	Defs();
public:
	double deltaTime;	// Delta time

	void UpdateDeltaTime()
	{
		currentFrame = glfwGetTime();			// Save the actual time
		deltaTime = currentFrame - lastFrame;	// Make a difference btw the actualFrame and the lastFrame
		lastFrame = currentFrame;				// Save the lastFrame with the actual time
	}

	static Defs* getInstance()
	{
		if (instance == NULL) instance = new Defs();
		return instance;
	}
	Defs (Defs const&) = delete;
	void operator = (Defs const&) = delete;
};