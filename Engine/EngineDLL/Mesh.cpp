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

	srand(time(0));

	fcData = FCCubeData();
	LoadMesh(modelPath, sTexturePath);

	this->texturePath = new char[sTexturePath.size() + 1];
	sTexturePath.copy(this->texturePath, sTexturePath.size() + 1);
	this->texturePath[sTexturePath.size()] = '\0';
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
		if (allBehind) return;
	}

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

	//DrawFCData();

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

bool Mesh::LoadMesh(const string& fileName, const string& textureName)
{
	bool state = ModelImporter::getInstance()->Import3DFromFile(fileName, textureName, m_Entries, m_Textures, fcData, renderer);

	for (int i = 0; i < m_Textures.size(); i++)
		bufferTextures.push_back(renderer->GenTexture(m_Textures[i].width, m_Textures[i].height, m_Textures[i].imageFormat, m_Textures[i].data));

	return state;
}