#pragma once
#include "Entity.h"

class Renderable : public Component
{
protected:
	Model *model;
	Effect *effect;
	void from_json(const nlohmann::json &j);

public:
	Renderable();
	~Renderable();

	void SetPlane(float spacing, unsigned int xSize, unsigned int ySize);
	void SetModel(std::string location);
	void SetEffect();
	void Render();
};