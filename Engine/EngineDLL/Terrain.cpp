#include "Terrain.h"
#include "PhysicsManager.h"
#include "GlobalDefs.h"
#include "Structs.h"
#include "Node.h"
#include "Mesh.h"

Terrain::Terrain()
{
	Start();
}
Terrain::~Terrain()
{
	DisposeHeightField();
}

void Terrain::Start()
{
	name = "Terrain";
	reqTransform = false;
}
void Terrain::Update()
{

}
void Terrain::Draw()
{

}
void Terrain::SetTransform(Transform* transform)
{

}

void Terrain::CreateHeightField(vector<vector<int>> sHeights, int rows, int columns, vec3 sScale)
{
	heights = sHeights;
	heightmapRows = rows;
	heightmapColumns = columns;
	scale = sScale;

	PxHeightFieldSample* samplesData = new PxHeightFieldSample[sizeof(PxHeightFieldSample) * rows * columns];
	PxHeightFieldDesc hfDesc;

	hfDesc.nbRows = (PxU32)heightmapRows;
	hfDesc.nbColumns = (PxU32)heightmapColumns;
	hfDesc.samples.data = samplesData;
	hfDesc.samples.stride = sizeof(PxHeightFieldSample);

	PxHeightFieldSample* currentSample = samplesData;

	for (int row = 0; row < heightmapRows; row++)
	{
		for (int col = 0; col < heightmapColumns; col++)
		{
			currentSample->height = (PxI16)heights[col][row];
			currentSample->materialIndex0 = 0;
			currentSample->materialIndex1 = 0;
			currentSample->clearTessFlag();
			currentSample++;
		}
	}

	PhysicsManager* physicsManager = PhysicsManager::getInstance();

	PxReal heightScale = (PxReal)scale.y / MAX_BYTE_VALUE;
	PxReal rowScale = (PxReal)scale.x;
	PxReal columnScale = (PxReal)scale.z;

	PxTransform relativePose(PxIdentity);

	heightField = physicsManager->CreateHeighField(hfDesc);
	geometry = new PxHeightFieldGeometry(heightField, PxMeshGeometryFlags(), heightScale, rowScale, columnScale);
	rigidActor = physicsManager->CreateRigidActor(relativePose, true);
	material = physicsManager->CreatePhysicsMaterial(0.1f, 0.1f, 0.1f);
	shape = PxRigidActorExt::createExclusiveShape(*rigidActor, *geometry, *material);

	physicsManager->AddActor(rigidActor);
}

void Terrain::DisposeHeightField()
{
	if (heightField)
		heightField->release();
	if (geometry)
		delete geometry;
	if (rigidActor)
	{
		if (material)
			material->release();
		if (shape)
			rigidActor->detachShape(*shape);
		PhysicsManager::getInstance()->RemoveActor(rigidActor);
		rigidActor->release();
	}
}

void Terrain::FlattenArea(Node* thisNode, int startRow, int endRow, int startColumn, int endColumn, int flattenHeight)
{
	vector<Vertex> vertices;

	flattenHeight = clamp(flattenHeight, 0, (int)MAX_BYTE_VALUE);

	for (int row = 0; row < heightmapRows; row++)
	{
		for (int col = 0; col < heightmapColumns; col++)
		{
			float posX = col * scale.x;
			float posZ = row * scale.z;

			float posY = 0.0f;

			if (row >= startRow && row <= endRow && col >= startColumn && col <= endColumn)
			{
				posY = flattenHeight / MAX_BYTE_VALUE * scale.y;
				heights[row][col] = flattenHeight;
			}
			else
				posY = (float)heights[row][col] / MAX_BYTE_VALUE * scale.y;

			float u = (float)col / (float)heightmapColumns;
			float v = 1.0f - (float)row / (float)heightmapRows;

			Vertex vertex(vec3(posX, posY, posZ),
				vec2(u, v),
				vec3(0.0f, 0.0f, 0.0f));

			vertices.push_back(vertex);
		}
	}

	CreateHeightField(heights, heightmapRows, heightmapColumns, scale);

	Mesh* mesh = (Mesh*)thisNode->GetComponent("Mesh");

	mesh->m_Entries.resize(1);
	mesh->m_Entries[0].Init(vertices, mesh->m_Entries[0].indices);
	mesh->SetDiffTex(mesh->m_Textures);
}