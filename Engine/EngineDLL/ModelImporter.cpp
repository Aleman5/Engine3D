#include "ModelImporter.h"

ModelImporter * ModelImporter::instance = NULL;

ModelImporter::ModelImporter()
{
}

void ModelImporter::Clear(vector<Header*>& m_Textures)
{
	for (unsigned int i = 0; i < m_Textures.size(); i++)
		SAFE_DELETE(m_Textures[i]);
}

bool ModelImporter::Import3DFromFile(const string& modelPath, vector<MeshEntry>& m_Entries, vector<Header*>& m_Textures, Renderer* renderer)
{
	// Release the previously loaded mesh (if it exists)
	Clear(m_Textures);

	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(modelPath.c_str(), ASSIMP_LOAD_FLAGS);

	if (pScene) {
		Ret = InitFromScene(pScene, modelPath, m_Entries, m_Textures, renderer);
	}
	else {
		printf("Error parsing '%s': '%s'\n", modelPath.c_str(), Importer.GetErrorString());
	}

	return Ret;
}

bool ModelImporter::InitFromScene(const aiScene* pScene, const string& Filename, vector<MeshEntry>& m_Entries, vector<Header*>& m_Textures, Renderer* renderer)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh, m_Entries, renderer);
	}

	return InitMaterials(pScene, Filename, m_Textures);
}

void ModelImporter::InitMesh(unsigned int Index, const aiMesh* paiMesh, vector<MeshEntry>& m_Entries, Renderer* renderer)
{
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	vector<Vertex> Vertices;
	vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Vertex v(vec3((float)pPos->x, (float)pPos->y, (float)pPos->z),
			vec2((float)pTexCoord->x, (float)pTexCoord->y),
			vec3((float)pNormal->x, (float)pNormal->y, (float)pNormal->z));

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

bool ModelImporter::InitMaterials(const aiScene* pScene, const string& Filename, vector<Header*>& m_Textures)
{
	// Extract the directory part from the file name
	/*string::size_type SlashIndex = Filename.find_last_of("/");
	string Dir;

	if (SlashIndex == string::npos)
	{
		Dir = ".";
	}
	else if (SlashIndex == 0)
	{
		Dir = "/";
	}
	else
	{
		Dir = Filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				std::string FullPath = Dir + "/" + Path.data;
				m_Textures[i] = new Header(GL_TEXTURE_2D, FullPath.c_str());

				if (!m_Textures[i]->Load())
				{
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				}
				else
				{
					printf("Loaded texture '%s'\n", FullPath.c_str());
				}
			}
		}

		// Load a white texture in case the model does not include its own texture
		if (!m_Textures[i])
		{
			m_Textures[i] = new Header(GL_TEXTURE_2D, "../Content/white.png");

			Ret = m_Textures[i]->Load();
		}
	}

	return Ret;*/

	return true;
}