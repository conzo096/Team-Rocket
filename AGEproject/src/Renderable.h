#pragma once
#include "Entity.h"
#include "Shader.h"

class Renderable : public Component
{
protected:
	Model *model;
	Effect *effect;
	void from_json(const nlohmann::json &j);

public:
	Renderable();
	~Renderable();

	Model GetModel() { return *model; }
	void SetPlane(float spacing, unsigned int xSize, unsigned int ySize);
	void SetModel(std::string location);
	void SetEffect(std::string texName);
	Effect* GetEffect() { return effect; }
	void Render();
};