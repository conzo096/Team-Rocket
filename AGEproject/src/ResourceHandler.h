/* This class handles all the resources from the game.
   Any entity that is created should have their data stored in these lists.
   They also must retrive their data from these lists.
   This is to ensure, no duplicate textures or models are being used.
   Furthermore, This allow for easier implementation of multi-threading in 
   the future. 
   
   Any questions ask Conner. However, his answer might not be suffice.
   */


#pragma once
#include "Singleton.h"
#include "GeometryUtil.h"
#include "Texture.h"
class Texture;
class ResourceHandler : public Singleton<ResourceHandler>
{
public:
	std::map<std::string, Model*> models;
	std::map<std::string, unsigned int> textures;
	std::map<std::string, GLShader*> shaders;
	std::map<std::string, char*> audio;

	void Load()
	{
		LoadModels();
		LoadTextures();
		LoadShaders();
		LoadAudio();
	}

	void LoadModels();
	void LoadTextures();
	void LoadShaders();
	void LoadAudio();

	// Find requested model. If it exists, return its index location.
	Model* GetModel(std::string model)
	{
		return models.find(model)->second;
	}

	// Find requested texture. If it exists, return its index location.
	unsigned int GetTexture(std::string tex)
	{
		return textures.find(tex)->second;
	}

	// Find requested shader. If it exists, return its index location.
	GLShader* GetShader(std::string shader)
	{
		return shaders.find(shader)->second;
	}

	// Find requested audio file. If it exists, return its index location.
	char* GetAudio(std::string sound)
	{
		return audio.find(sound)->second;
	}
};


