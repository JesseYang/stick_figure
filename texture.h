#pragma once

#include <string>

#include <GL/glew.h>
#include <Magick++.h>

class Texture
{
public:
	Texture(GLenum TextureTarget, const std::string& FileName);

	bool Load();
	bool LoadBMP();

	void Bind(GLenum TextureUnit);

private:
	std::string m_fileName;
	GLenum m_textureTarget;
	GLuint m_textureObj;
	Magick::Image m_image;
	Magick::Blob m_blob;
};