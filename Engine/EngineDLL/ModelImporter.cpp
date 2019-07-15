#include "ModelImporter.h"
#include "Node.h"
#include "Mesh.h"

ModelImporter * ModelImporter::instance = NULL;

ModelImporter::ModelImporter()
{
}

void ModelImporter::Load(Node* thisNode, const string modelPath, const string texturePath, Material* material)
{
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(modelPath.c_str(), ASSIMP_LOAD_FLAGS_TRIANG_FLIP);

	if (!pScene)
		printf("Error parsing '%s': '%s'\n", modelPath.c_str(), Importer.GetErrorString());

	AttendNode(pScene, pScene->mRootNode, thisNode, thisNode->fcData, modelPath, texturePath, material);

	thisNode->fcData.UpdateData();
}

bool ModelImporter::AttendNode(const aiScene* aiScene, aiNode* aiNode, Node* parent,
							   FCCubeData& fcData, const string modelPath, const string texturePath, Material* material)
{
	for (int i = 0; i < (int)aiNode->mNumMeshes; i++)
	{
		const aiMesh* aiMesh = aiScene->mMeshes[aiNode->mMeshes[i]];

		Node* child = new Node(aiNode->mName.C_Str(), parent);
		child->AddComponent(InitMesh(aiScene, aiMesh, parent, fcData, modelPath, texturePath, i, material));
		//cout << "Added child " << child->GetName() << ", the parent is " << parent->GetName() << endl;
	}

	for (int i = 0; i < (int)aiNode->mNumChildren; i++)
	{
		AttendNode(aiScene, aiNode->mChildren[i], parent, fcData, modelPath, texturePath, material);
	}

	return true;
}

Mesh* ModelImporter::InitMesh(const aiScene* aiScene, const aiMesh* aiMesh, Node* parent,
							 FCCubeData& fcData, const string modelPath, const string texturePath, unsigned int Index, Material* material)
{
	Mesh* mesh = new Mesh(material);

	mesh->m_Entries.resize(aiScene->mNumMeshes);
	mesh->m_Textures.resize(aiScene->mNumMaterials);

	mesh->m_Entries[Index].MaterialIndex = aiMesh->mMaterialIndex;

	vector<Vertex> Vertices;
	vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	float minX = 999999.0f, minY = 999999.0f, minZ = 999999.0f;
	float maxX = -999999.0f, maxY = -999999.0f, maxZ = -999999.0f;

	for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(aiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(aiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = aiMesh->HasTextureCoords(0) ? &(aiMesh->mTextureCoords[0][i]) : &Zero3D;

		Vertex v(vec3((float)pPos->x, (float)pPos->y, (float)pPos->z),
			vec2((float)pTexCoord->x, (float)pTexCoord->y),
			vec3((float)pNormal->x, (float)pNormal->y, (float)pNormal->z));

		fcData.NewValue(v.m_pos);

		Vertices.push_back(v);
	}

	for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
	{
		const aiFace& Face = aiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	mesh->m_Entries[Index].Init(Vertices, Indices);

	for (unsigned int i = 0; i < aiScene->mNumMaterials; i++)
	{
		mesh->m_Textures[i] = TextureImporter::LoadImage(texturePath.c_str());
		mesh->bufferTextures.push_back(Renderer::getInstance()->GenTexture(mesh->m_Textures[i].width, mesh->m_Textures[i].height, mesh->m_Textures[i].imageFormat, mesh->m_Textures[i].data));
	}

	return mesh;
}

void ModelImporter::InitTexture()
{
	//for (int i = 0; i < m_Textures.size(); i++)
	//	bufferTextures.push_back(renderer->GenTexture(m_Textures[i].width, m_Textures[i].height, m_Textures[i].imageFormat, m_Textures[i].data));
}