#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Exports.h"
#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\glm.hpp>

class ENGINEDLL_API ModelImporter
{
	static ModelImporter *instance;

	const aiScene* scene = NULL;
	Assimp::Importer importer;
	
public:
	bool Import3DFromFile(const std::string& modelPath, glm::vec3& vertices, unsigned int& indices);

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

