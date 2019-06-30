#include "TextureImporter.h"
#include "SOIL.h"

TextureImporter::TextureImporter()
{

}
TextureImporter::~TextureImporter()
{

}

Header TextureImporter::loadBMP_custom(const char* imagePath)
{
	static unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	Header h;

	FILE * file;
	fopen_s(&file, imagePath, "rb");
	
	fread_s(header, 54, 1, 54, file);

	switch (CheckFormat(file, header))
	{
	case COULDNT_OPEN:

		break;
	case INCORRECT_FORMAT:

		break;
	case BMP_FORMAT:
		h.dataPos = *(int*)&(header[0x0A]);
		h.imageSize = *(int*)&(header[0x22]);
		h.width = *(int*)&(header[0x12]);
		h.height = *(int*)&(header[0x16]);

		// Algunos archivos BMP tienen un mal formato, así que adivinamos la información faltante
		if (h.imageSize == 0)    h.imageSize = h.width * h.height * 3; // 3 : un byte por cada componente Rojo (Red), Verde (Green) y Azul(Blue)
		if (h.dataPos == 0)      h.dataPos = 54; // El encabezado del BMP esta hecho de esta manera

		// Buffer is created
		h.data = new unsigned char[h.imageSize];
		h.imageFormat = GL_RGB;

		// We read the file's information and put it into the buffer
		fseek(file, h.dataPos, 0);
		fread_s(h.data, h.imageSize, 1, h.imageSize, file);
		break;
	case PNG_FORMAT:

		break;
	}
	
	fclose(file);

	return h;
}

Header TextureImporter::LoadImage(const char* imagePath)
{
	Header h;
	
	int channels;
	h.data = SOIL_load_image(imagePath, (int*)&h.width, (int*)&h.height, &channels, 0);

	if (!h.data)
	{
		SOIL_free_image_data(h.data);
		throw logic_error("The image file could not be loaded.");
	}

	cout << "Reading the image " << imagePath << endl;

	switch (channels)
	{
	case 1:
		h.imageFormat = GL_RED;
		if (h.imageSize == 0)    h.imageSize = h.width * h.height;
		break;
	case 3:
		h.imageFormat = GL_RGB;
		if (h.imageSize == 0)    h.imageSize = h.width * h.height * 3;
		break;
	case 4:
		h.imageFormat = GL_RGBA;
		if (h.imageSize == 0)    h.imageSize = h.width * h.height * 4;
		break;
	}

	return h;
}

int TextureImporter::CheckFormat(FILE*& file, unsigned char header[])
{
	if (!file) { cout << "Image could not be opened\n" << endl; return COULDNT_OPEN; }

	if (header[0] == 'B' || header[1] == 'M') {
		cout << "A correct BMP file\n" << endl;
		return BMP_FORMAT;
	}

	return INCORRECT_FORMAT;
}