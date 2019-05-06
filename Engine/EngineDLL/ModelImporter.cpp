#include "ModelImporter.h"

ModelImporter * ModelImporter::instance = NULL;

ModelImporter::ModelImporter()
{
}

bool ModelImporter::Import3DFromFile(const std::string& modelPath)
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

	// If the import failed, report it
	if (!scene)
	{
		printf(importer.GetErrorString());
		return false;
	}

	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}