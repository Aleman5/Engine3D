#include "ModelImporter.h"

ModelImporter * ModelImporter::instance = NULL;

ModelImporter::ModelImporter()
{
}

bool ModelImporter::Import3DFromFile(const string& modelPath, const string& texturePath, vector<MeshEntry>& m_Entries, vector<Header>& m_Textures, FCCubeData& fcData, Renderer* renderer)
{
	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(modelPath.c_str(), ASSIMP_LOAD_FLAGS_TRIANG_FLIP);

	if (pScene)
		Ret = InitFromScene(pScene, texturePath, m_Entries, m_Textures, fcData, renderer);
	else
		printf("Error parsing '%s': '%s'\n", modelPath.c_str(), Importer.GetErrorString());

	return Ret;
}

bool ModelImporter::InitFromScene(const aiScene* pScene, const string& texturePath, vector<MeshEntry>& m_Entries, vector<Header>& m_Textures, FCCubeData& fcData, Renderer* renderer)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		
		InitMesh(i, paiMesh, m_Entries, fcData, renderer);
	}

	// Init of the Textures
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
		m_Textures[i] = TextureImporter::LoadImage(texturePath.c_str());

	return true;
}

void ModelImporter::InitMesh(unsigned int Index, const aiMesh* paiMesh, vector<MeshEntry>& m_Entries, FCCubeData& fcData, Renderer* renderer)
{
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	vector<Vertex> Vertices;
	vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	float minX =  999999.0f, minY =  999999.0f, minZ =  999999.0f;
	float maxX = -999999.0f, maxY = -999999.0f, maxZ = -999999.0f;

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Vertex v(vec3((float)pPos->x, (float)pPos->y, (float)pPos->z),
			vec2((float)pTexCoord->x, (float)pTexCoord->y),
			vec3((float)pNormal->x, (float)pNormal->y, (float)pNormal->z));

		if (pPos->x > maxX) maxX = pPos->x;
		if (pPos->x < minX) minX = pPos->x;
		if (pPos->y > maxY) maxY = pPos->y;
		if (pPos->y < minY) minY = pPos->y;
		if (pPos->z > maxZ) maxZ = pPos->z;
		if (pPos->z < minZ) minZ = pPos->z;

		fcData = FCCubeData(minX, minY, minZ, maxX, maxY, maxZ);

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

	m_Entries[Index].Init(Vertices, Indices, renderer);
}