#pragma once

#include <fstream>
#include "Renderer.h"

#define COULDNT_OPEN -2
#define INCORRECT_FORMAT -1
#define BMP_FORMAT 0
#define PNG_FORMAT 1

struct Header {
	unsigned int id;
	unsigned int dataPos;		// Position of the Data
	unsigned int width, height;	// Width and Height of the image
	unsigned int imageSize;		// Size of the image
	unsigned int imageFormat;	// Format of the image
	unsigned char * data;		// Data of the image
};
static class TextureImporter
{
	static int CheckFormat(
		FILE*& file,			// File of the image
		unsigned char header[]	// Temp Header
	);

public:
	static Header loadBMP_custom(
		const char* imagePath	// Path of the image
	);
	static Header LoadImage(
		const char* imagePath
	);

	TextureImporter();
	~TextureImporter();
};

