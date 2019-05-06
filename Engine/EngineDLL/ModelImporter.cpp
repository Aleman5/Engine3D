#include "ModelImporter.h"

ModelImporter::ModelImporter()
{
}

bool ModelImporter::Import3DFromFile(const char& modelPath)
{
	// Check if file exists
	std::ifstream fin(modelPath.c_str());
	if (!fin.fail())
	{
		fin.close();
	}
	else
	{
		MessageBox(NULL, ("Couldn't open file: " + pFile).c_str(), "ERROR", MB_OK | MB_ICONEXCLAMATION);
		logInfo(importer.GetErrorString());
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