#pragma once
#include "Entity.h"
#include "Shader.h"
#include "ResourceHandler.h"
class Renderable : public Component
{
protected:
	Model *model;
	Effect *effect;

	// render info that gets sent to the game engine renderer.
	RenderData renderInfo;
	void from_json(const nlohmann::json &j);

public:
	Renderable();
	~Renderable();

	Model GetModel() { return *model; }
	void SetPlane(float spacing, unsigned int xSize, unsigned int ySize);
	void SetModel(std::string location);
	void SetTexture(std::string texName);
	void SetMaterial(Material mat);
	void SetShader(std::string shader);
	Effect* GetEffect() { return effect; }
	void Render();
};