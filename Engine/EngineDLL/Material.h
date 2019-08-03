#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Exports.h"
#include "TextureImporter.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "glm/glm.hpp"

using namespace glm;

class ENGINEDLL_API Material
{
	Header* header;

	unsigned int matrixId;		// Id of the Matrix
	unsigned int programId;		// Id of the Program
	unsigned int textureId;		// Id of the Texture (Header)

	unsigned int LoadShader(
		const char * vertex_file_path,	// Path of the vexter file
		const char * fragment_file_path	// Path of the fragment file
	);

public:
	static Material* GenerateMaterial(
		const char * vertex_file_path,	// Path of the vexter file
		const char * fragment_file_path	// Path of the fragment file
	);
	static void DestroyMaterial(Material* material);
	
	void Bind();
	void Bind(const char* texName, unsigned int texture);
	void BindTexture();
	void BindTexture(unsigned int texture);
	void SetMatrixProperty(
		const char* name,	// Name of the matrix to fill a material
		mat4& mat		// Value of the matrix
	);
	void SetTexture(
		Header* header,
		const char* propertyName
	);

	Material();
	~Material();
};

