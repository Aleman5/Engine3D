#pragma once

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include "Exports.h"
#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"

class ENGINEDLL_API ModelImporter
{
	static ModelImporter *instance;

	const aiScene* scene = NULL;
	
public:
	bool Import3DFromFile(const char& modelPath);

	static ModelImporter* getInstance()
	{
		if (instance == NULL) instance = new ModelImporter();
		return instance;
	}
	ModelImporter(ModelImporter const&) = delete;
	void operator = (ModelImporter const&) = delete;

private:
	ModelImporter();
};

