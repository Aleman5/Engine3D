#include "ModelImporter.h"

ModelImporter * ModelImporter::instance = NULL;

ModelImporter::ModelImporter()
{
}

bool ModelImporter::Import3DFromFile(const std::string& modelPath, glm::vec3& vertices, unsigned int& indices)
{
	// Check if file exists
	std::ifstream fin(modelPath);
	if (!fin.fail())
	{
		fin.close();
	}
	else
	{
		printf("Couldn't open file");
		printf(importer.GetErrorString());
		return false;
	}

	scene = importer.ReadFile(modelPath, aiProcessPreset_TargetRealtime_Quality);

	//vertices = scene->mMeshes[0]->mVertices;
	//scene->mMeshes[0]->mFaces->mIndices;
	
	// If the import failed, report it
	if (!scene)
	{
		printf(importer.GetErrorString());
		return false;
	}

	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}