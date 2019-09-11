#pragma once

#include "Exports.h"
#include "Renderer.h"
#include "GlobalDefs.h"
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
		const vector<unsigned int>& Indices
	);

	unsigned int verticesBuffer;
	unsigned int indicesBuffer;
	unsigned int uvBuffer;

	vector<unsigned int> indices;

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

	vec3* vertex;

	bool initialized = false;

	float lMinX = 999999.0f, lMinY = 999999.0f, lMinZ = 999999.0f;
	float lMaxX = -999999.0f, lMaxY = -999999.0f, lMaxZ = -999999.0f;

	FCCubeData() { vertex = new vec3[CUBE_VERTICES]; }

	void UpdateData()
	{
	  /*Top_Right_Far.x = maxX;		Top_Right_Near.x = maxX;	 Top_Left_Far.x = minX;		Top_Left_Near.x = minX;
		Top_Right_Far.y = maxY;		Top_Right_Near.y = maxY;	 Top_Left_Far.y = maxY;		Top_Left_Near.y = maxY;
		Top_Right_Far.z = maxZ;		Top_Right_Near.z = minZ;	 Top_Left_Far.z = maxZ;		Top_Left_Near.z = minZ;

		Bottom_Right_Far.x = maxX;  Bottom_Right_Near.x = maxX;  Bottom_Left_Far.x = minX;  Bottom_Left_Near.x = minX;
		Bottom_Right_Far.y = minY;	Bottom_Right_Near.y = minY;	 Bottom_Left_Far.y = minY;	Bottom_Left_Near.y = minY;
		Bottom_Right_Far.z = maxZ;	Bottom_Right_Near.z = minZ;	 Bottom_Left_Far.z = maxZ;	Bottom_Left_Near.z = minZ;*/

		//vertex = new vec3[8];

		vertex[0].x = lMaxX; vertex[1].x = lMaxX;  vertex[2].x = lMinX;  vertex[3].x = lMinX;
		vertex[0].y = lMaxY; vertex[1].y = lMaxY;  vertex[2].y = lMaxY;  vertex[3].y = lMaxY;
		vertex[0].z = lMaxZ; vertex[1].z = lMinZ;  vertex[2].z = lMaxZ;  vertex[3].z = lMinZ;
		
		vertex[4].x = lMaxX; vertex[5].x = lMaxX;  vertex[6].x = lMinX;  vertex[7].x = lMinX;
		vertex[4].y = lMinY; vertex[5].y = lMinY;  vertex[6].y = lMinY;  vertex[7].y = lMinY;
		vertex[4].z = lMaxZ; vertex[5].z = lMinZ;  vertex[6].z = lMaxZ;  vertex[7].z = lMinZ;

		initialized = true;
	}

	void CompareData(FCCubeData childFcData)
	{
		if (childFcData.vertex[0].x > vertex[0].x) // MaxX
		{
			vertex[0].x = childFcData.vertex[0].x;
			vertex[1].x = childFcData.vertex[1].x;
			vertex[4].x = childFcData.vertex[4].x;
			vertex[5].x = childFcData.vertex[5].x;
		}

		if (childFcData.vertex[0].y > vertex[0].y) // MaxY
		{
			vertex[0].y = childFcData.vertex[0].y;
			vertex[1].y = childFcData.vertex[1].y;
			vertex[2].y = childFcData.vertex[2].y;
			vertex[3].y = childFcData.vertex[3].y;
		}

		if (childFcData.vertex[0].z > vertex[0].z) // MaxZ
		{
			vertex[0].z = childFcData.vertex[0].z;
			vertex[2].z = childFcData.vertex[2].z;
			vertex[4].z = childFcData.vertex[4].z;
			vertex[6].z = childFcData.vertex[6].z;
		}

		if (childFcData.vertex[7].x < vertex[7].x) // MinX
		{
			vertex[2].x = childFcData.vertex[2].x;
			vertex[3].x = childFcData.vertex[3].x;
			vertex[6].x = childFcData.vertex[6].x;
			vertex[7].x = childFcData.vertex[7].x;
		}

		if (childFcData.vertex[7].y < vertex[7].y) // MinY
		{
			vertex[4].y = childFcData.vertex[4].y;
			vertex[5].y = childFcData.vertex[5].y;
			vertex[6].y = childFcData.vertex[6].y;
			vertex[7].y = childFcData.vertex[7].y;
		}

		if (childFcData.vertex[7].z < vertex[7].z) // MinZ
		{
			vertex[1].z = childFcData.vertex[1].z;
			vertex[3].z = childFcData.vertex[3].z;
			vertex[5].z = childFcData.vertex[5].z;
			vertex[7].z = childFcData.vertex[7].z;
		}
	}

	void NewValue(const vec3 newValue)
	{
		if (newValue.x > lMaxX) lMaxX = newValue.x;
		if (newValue.x < lMinX) lMinX = newValue.x;
		if (newValue.y > lMaxY) lMaxY = newValue.y;
		if (newValue.y < lMinY) lMinY = newValue.y;
		if (newValue.z > lMaxZ) lMaxZ = newValue.z;
		if (newValue.z < lMinZ) lMinZ = newValue.z;
	}
};