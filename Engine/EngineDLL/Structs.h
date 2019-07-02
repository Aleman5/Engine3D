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
	unsigned int uvBuffer;


	unsigned int NumIndices;
	unsigned int MaterialIndex;

	MeshEntry();
	~MeshEntry();
};

struct FCCubeData
{
	/*vec3 Top_Right_Far;	0
	vec3 Top_Right_Near;	1
	vec3 Top_Left_Far;		2
	vec3 Top_Left_Near;		3
	vec3 Bottom_Right_Far;	4
	vec3 Bottom_Right_Near;	5
	vec3 Bottom_Left_Far;	6
	vec3 Bottom_Left_Near;  7 */

	vec4* vertex;

	float minX = 999999.0f, minY = 999999.0f, minZ = 999999.0f;
	float maxX = -999999.0f, maxY = -999999.0f, maxZ = -999999.0f;

	FCCubeData() { vertex = new vec4[8]; }

	void UpdateData()
	{
	  /*Top_Right_Far.x = maxX;		Top_Right_Near.x = maxX;	 Top_Left_Far.x = minX;		Top_Left_Near.x = minX;
		Top_Right_Far.y = maxY;		Top_Right_Near.y = maxY;	 Top_Left_Far.y = maxY;		Top_Left_Near.y = maxY;
		Top_Right_Far.z = maxZ;		Top_Right_Near.z = minZ;	 Top_Left_Far.z = maxZ;		Top_Left_Near.z = minZ;

		Bottom_Right_Far.x = maxX;  Bottom_Right_Near.x = maxX;  Bottom_Left_Far.x = minX;  Bottom_Left_Near.x = minX;
		Bottom_Right_Far.y = minY;	Bottom_Right_Near.y = minY;	 Bottom_Left_Far.y = minY;	Bottom_Left_Near.y = minY;
		Bottom_Right_Far.z = maxZ;	Bottom_Right_Near.z = minZ;	 Bottom_Left_Far.z = maxZ;	Bottom_Left_Near.z = minZ;*/

		vertex = new vec4[8];

		vertex[0].x = maxX;	 vertex[1].x = maxX;  vertex[2].x = minX;  vertex[3].x = minX;
		vertex[0].y = maxY;	 vertex[1].y = maxY;  vertex[2].y = maxY;  vertex[3].y = maxY;
		vertex[0].z = maxZ;	 vertex[1].z = minZ;  vertex[2].z = maxZ;  vertex[3].z = minZ;
		vertex[0].w = 1.0f;	 vertex[1].w = 1.0f;  vertex[2].w = 1.0f;  vertex[3].w = 1.0f;
							 					 					  
		vertex[4].x = maxX;  vertex[5].x = maxX;  vertex[6].x = minX;  vertex[7].x = minX;
		vertex[4].y = minY;	 vertex[5].y = minY;  vertex[6].y = minY;  vertex[7].y = minY;
		vertex[4].z = maxZ;	 vertex[5].z = minZ;  vertex[6].z = maxZ;  vertex[7].z = minZ;
		vertex[4].w = 1.0f;	 vertex[5].w = 1.0f;  vertex[6].w = 1.0f;  vertex[7].w = 1.0f;
	}

	void NewValue(const vec3 newValue)
	{
		if (newValue.x > maxX) maxX = newValue.x;
		if (newValue.x < minX) minX = newValue.x;
		if (newValue.y > maxY) maxY = newValue.y;
		if (newValue.y < minY) minY = newValue.y;
		if (newValue.z > maxZ) maxZ = newValue.z;
		if (newValue.z < minZ) minZ = newValue.z;
	}
};