/*#include "ModelImporter.h"

ModelImporter * ModelImporter::instance = NULL;

ModelImporter::ModelImporter()
{
}

bool ModelImporter::Import3DFromFile(Mesh* mesh)
{
	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(mesh->modelPath.c_str(), ASSIMP_LOAD_FLAGS_TRIANG_FLIP);

	if (pScene)
		Ret = InitFromScene(pScene, mesh);
	else
		printf("Error parsing '%s': '%s'\n", mesh->modelPath.c_str(), Importer.GetErrorString());

	return Ret;
}

bool ModelImporter::InitFromScene(const aiScene* pScene, Mesh* mesh)
{
	mesh->m_Entries.resize(pScene->mNumMeshes);
	mesh->m_Textures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < mesh->m_Entries.size(); i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];

		InitMesh(i, paiMesh, mesh);
	}

	mesh->fcData.UpdateData();

	// Init of the Textures
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
		mesh->m_Textures[i] = TextureImporter::LoadImage(mesh->texturePath);

	return true;
}

void ModelImporter::InitMesh(unsigned int Index, const aiMesh* paiMesh, Mesh* mesh)
{
	mesh->m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	vector<Vertex> Vertices;
	vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	float minX = 999999.0f, minY = 999999.0f, minZ = 999999.0f;
	float maxX = -999999.0f, maxY = -999999.0f, maxZ = -999999.0f;

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Vertex v(vec3((float)pPos->x, (float)pPos->y, (float)pPos->z),
			vec2((float)pTexCoord->x, (float)pTexCoord->y),
			vec3((float)pNormal->x, (float)pNormal->y, (float)pNormal->z));

		mesh->fcData.NewValue(v.m_pos);

		Vertices.push_back(v);
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	mesh->m_Entries[Index].Init(Vertices, Indices);
}*/