#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <iostream>

class Texture
{
private:
	int WIDTH = NULL;
	int HEIGHT = NULL;
	int NRCHANNELS = NULL;

public:
	unsigned int ID = NULL;
	GLenum TextureUnit = NULL;

	Texture(std::string filename,
		GLenum textureUnit,
		GLenum internalFormat,
		GLenum format,
		bool stbi_set_flip_vertically_on_load = true,
		GLenum wrapS = GL_REPEAT, GLenum wrapT = GL_REPEAT, GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR);
};

#endif