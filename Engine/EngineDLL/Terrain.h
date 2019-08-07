#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <PxPhysicsAPI.h>
#include "Exports.h"
#include "Component.h"

using namespace std;
using namespace glm;
using namespace physx;

class Node;

class ENGINEDLL_API Terrain : public Component
{
	PxHeightField* heightField;
	PxRigidActor* rigidActor;
	PxHeightFieldGeometry* geometry;
	PxMaterial* material;
	PxShape* shape;

	int heightmapRows;
	int heightmapColumns;
	vector<vector<int>> heights;
	vec3 scale;

	void DisposeHeightField();

public:
	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;

	void CreateHeightField(vector<vector<int>> sHeights, int rows, int columns, vec3 sScale);

	void FlattenArea(Node* thisNode, int startRow, int endRow, int startColumn, int endColumn, int height);

	inline int GetHeightmapRows() const { return heightmapRows; }
	inline int GetHeightmapColumns() const { return heightmapColumns; }
	inline vec3 GetScale() const { return scale; }

	Terrain();
	~Terrain();
};