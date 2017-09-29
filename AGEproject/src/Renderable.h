#pragma once
#include "Entity.h"
#include "Model.h"
#include "Material.h"

class Renderable : public Component
{
	struct Effect
	{
		unsigned int texture;
		std::string shader;
		Material material;
	};
protected:
	Model *model;
	Effect *effect;
	void from_json(const nlohmann::json &j);

public:
	Renderable();
	~Renderable();

	void SetModel(std::string location);
	void SetEffect();
	void Render();
};