#pragma once
#include "GL\glew.h"
#include "stb_image.h"
#include "FileIO.h"

#include <iostream>
class Texture
{
private:
	// Id of the texture on the gpu.
	unsigned int textureId;
	// Dimensions of the texture.
	int width=0, height=0;
	// Type of texture;
	GLenum textureType;

public:

	Texture() {}
	Texture(int w, int h);
	Texture(const char* fileLocation);

	void SetType(GLenum type)
	{
		textureType = type;
	}

	unsigned int& GetTextureId()
	{
		return textureId;
	}

	unsigned int GetCopiedTextureId()
	{
		return textureId;
	}
	GLenum& GetTextureType()
	{
		return textureType;
	}
};