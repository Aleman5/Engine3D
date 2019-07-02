#include "Mesh.h"

Mesh::Mesh(Material* material, const string modelPath, const string texturePath)
	: material(material), modelPath(modelPath), sTexturePath(texturePath)
{
	Start();
}
Mesh::~Mesh()
{
	if (texturePath) delete texturePath;
}

void Mesh::Start()
{
	name = "Mesh";
	reqTransform = true;

	renderer = Renderer::getInstance();

	fcData = FCCubeData();

	this->texturePath = new char[sTexturePath.size() + 1];
	sTexturePath.copy(this->texturePath, sTexturePath.size() + 1);
	this->texturePath[sTexturePath.size()] = '\0';
	
	Import3DFromFile();

	debugMode = false;
}

void Mesh::Update()
{
	
}

void Mesh::Draw()
{
	Plane* planes = renderer->GetPlanes();
	
	for (int i = 0; i < 6; i++)
	{
		bool allBehind = true;

		for (int j = 0; j < 8; j++)
		{
			if (renderer->ClassifyPoint(planes[i], fcData.vertex[j] * renderer->GetModelMatrix()) == POSITIVE)
			{
				allBehind = false;
				break;
			}
		}
		if (allBehind)
		{
			if (debugMode) cout << "Not drawing" << endl;
			return;
		}
	}

	if (debugMode) cout << "Drawing" << endl;

	if (material != NULL)
	{
		material->Bind("myTextureSampler", bufferTextures[0]);
		material->SetMatrixProperty("MVP", renderer->GetMVP());
	} 

	renderer->EnableAttributes(0);
	renderer->EnableAttributes(1);

	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		renderer->BindBuffer(m_Entries[i].verticesBuffer, 0);
		renderer->BindTextureBuffer(m_Entries[i].uvBuffer, 1);
		renderer->BindElementBuffer(m_Entries[i].indicesBuffer);
		renderer->DrawElementBuffer(m_Entries[i].NumIndices);
	}

	if (debugMode) DrawFCData();

	renderer->DisableAttributes(0);
	renderer->DisableAttributes(1);
}

void Mesh::DrawFCData()
{
	FCCubeData fcNew = FCCubeData();

	for (int i = 0; i < 8; i++)
	{
		fcNew.vertex[i] = fcData.vertex[i];
	}

	float* fcVertex = new float[12*3]
	{
		fcNew.vertex[0].x, fcNew.vertex[0].y, fcNew.vertex[0].z,
		fcNew.vertex[1].x, fcNew.vertex[1].y, fcNew.vertex[1].z,
		fcNew.vertex[4].x, fcNew.vertex[4].y, fcNew.vertex[4].z,
						   					  
		fcNew.vertex[1].x, fcNew.vertex[1].y, fcNew.vertex[1].z,
		fcNew.vertex[4].x, fcNew.vertex[4].y, fcNew.vertex[4].z,
		fcNew.vertex[5].x, fcNew.vertex[5].y, fcNew.vertex[5].z,
											  
		fcNew.vertex[2].x, fcNew.vertex[2].y, fcNew.vertex[2].z,
		fcNew.vertex[3].x, fcNew.vertex[3].y, fcNew.vertex[3].z,
		fcNew.vertex[6].x, fcNew.vertex[6].y, fcNew.vertex[6].z,
											  
		fcNew.vertex[3].x, fcNew.vertex[3].y, fcNew.vertex[3].z,
		fcNew.vertex[6].x, fcNew.vertex[6].y, fcNew.vertex[6].z,
		fcNew.vertex[7].x, fcNew.vertex[7].y, fcNew.vertex[7].z,
	};

	float* verticesColorData = new float[3*12]{
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};

	vector<unsigned int> indices{
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9,10,11
	};

	unsigned int id = renderer->GenBuffer(fcVertex, sizeof(float) * 12 * 3);
	unsigned int colorId = renderer->GenBuffer(verticesColorData, sizeof(float) * 12 * 3);
	unsigned int elementsId = renderer->GenElementBuffer(indices);

	renderer->BindBuffer(id, 0);
	renderer->BindTextureBuffer(colorId, 1);
	renderer->BindElementBuffer(elementsId);
	renderer->DrawElementBuffer(elementsId);
}

void Mesh::SetTransform(Transform* transform)
{
	this->transform = transform;
}

void Mesh::ActivateDebugMode()
{
	debugMode = true;
}

void Mesh::DesactivateDebugMode()
{
	debugMode = false;
}

bool Mesh::Import3DFromFile()
{
	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(modelPath.c_str(), ASSIMP_LOAD_FLAGS_TRIANG_FLIP);

	if (pScene)
		Ret = InitFromScene(pScene);
	else
		printf("Error parsing '%s': '%s'\n", modelPath.c_str(), Importer.GetErrorString());

	return Ret;
}

bool Mesh::InitFromScene(const aiScene* pScene)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];

		InitMesh(i, paiMesh);
	}

	fcData.UpdateData();

	// Init of the Textures
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
		m_Textures[i] = TextureImporter::LoadImage(texturePath);
	
	InitTexture(); // For the textures I should do something like the meshes.

	return true;
}

void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

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

		fcData.NewValue(v.m_pos);

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

	m_Entries[Index].Init(Vertices, Indices);
}

void Mesh::InitTexture()
{
	for (int i = 0; i < m_Textures.size(); i++)
		bufferTextures.push_back(renderer->GenTexture(m_Textures[i].width, m_Textures[i].height, m_Textures[i].imageFormat, m_Textures[i].data));
}