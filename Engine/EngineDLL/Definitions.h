#pragma once

#include "Exports.h"
#include "GL\glew.h"

#define GLM_ENABLE_EXPERIMENTAL

#include "GLFW\glfw3.h"

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

struct Vertex
{
	vec3 m_pos;
	vec2 m_tex;
	vec3 m_normal;

	Vertex() {}

	Vertex(const vec3& pos, const vec2& tex, const vec3& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};

struct MeshEntry
{
	void Init(
		const vector<Vertex>& Vertices,
		const vector<unsigned int>& Indices,
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