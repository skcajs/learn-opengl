#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(std::string filename,
	GLenum textureUnit,
	GLenum internalFormat,
	GLenum format,
	bool stbi_set_flip_vertically_on_load,
	GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter)
{
	TextureUnit = textureUnit;
	glGenTextures(1, &ID);
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_2D, ID);
	// set texture wrapping/filtering options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	// load and generate the texture
	unsigned char* data = stbi_load(filename.c_str(), &WIDTH, &HEIGHT, &NRCHANNELS, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, WIDTH, HEIGHT, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}