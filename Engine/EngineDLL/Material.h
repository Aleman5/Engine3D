#pragma once

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include "Exports.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "glm/glm.hpp"

using namespace glm;

class ENGINEDLL_API Material
{
	GLuint matrixId;		// Id of the Matrix
	unsigned int programId;	// Id of the Program
public:
	unsigned int LoadShader(
		const char * vertex_file_path,	// Path of the vexter file
		const char * fragment_file_path	// Path of the fragment file
	);
	
	void Bind();
	void Bind(const char* texName, unsigned int texture);
	void BindTexture(unsigned int texture);
	void SetMatrixProperty(
		const char* name,	// Name of the matrix to fill a material
		glm::mat4& mat		// Value of the matrix
	);

	Material();
	~Material();
};

