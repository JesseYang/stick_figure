#include <iostream>
#include "texture.h"


Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
	m_textureTarget = TextureTarget;
	m_fileName = FileName;
}

bool Texture::LoadBMP()
{
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	// FILE * file = fopen(m_fileName.c_str(), "rb");
	FILE * file;
	fopen_s(&file, m_fileName.c_str(), "rb");
	if (!file)
	{
		std::cout << "Error loading texture " << m_fileName << std::endl;
		return false;
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return false;
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return false;
	}

	/*
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0)
	{
		printf("Not a correct BMP file\n");
		return false;
	}
	if (*(int*)&(header[0x1C]) != 24)
	{
		printf("Not a correct BMP file\n");
		return false;
	}
	*/

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)
		imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)
		dataPos = 54; // The BMP header is done that wa

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	// Everything is in memory now, the file wan be closed
	fclose(file);
	
	// Create one OpenGL texture
	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj);
	glTexImage2D(m_textureTarget, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(m_textureTarget, 0);

	return true;
}

bool Texture::Load()
{
	try {
		m_image.read(m_fileName);
		m_image.write(&m_blob, "RGBA");
	}
	catch (Magick::Error& Error) {
		std::cout << "Error loading texture '" << m_fileName << "': " << Error.what() << std::endl;
		return false;
	}

	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj);
	glTexImage2D(m_textureTarget, 0, GL_RGBA, m_image.columns(), m_image.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(m_textureTarget, 0);

	return true;
}

void Texture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(m_textureTarget, m_textureObj);
}
