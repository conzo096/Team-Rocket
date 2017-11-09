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

	// Better way to handle this?
	//std::vector<Component*> componentList;

	ResourceHandler() 
	{
		LoadModels();
		LoadTextures();
		LoadShaders();
	}

	~ResourceHandler()
	{
	
	}

	void LoadModels();
	void LoadTextures();
	void LoadShaders();

	// Find requested model. If it exists return its index location.
	Model* GetModel(std::string model)
	{
		return models.find(model)->second;
	}

	unsigned int GetTexture(std::string tex)
	{
		return textures.find(tex)->second;
	}

	GLShader* GetShader(std::string shader)
	{
		return shaders.find(shader)->second;
	}

};


