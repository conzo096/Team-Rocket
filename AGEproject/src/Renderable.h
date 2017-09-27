#pragma once
#include "Entity.h"
#include "Model.h"

class Renderable : public Component
{
	struct Effect
	{
		unsigned int texture;
		//add shaders and material
	};
protected:
	Model *mesh;
	Effect *effect;
	void from_json(const nlohmann::json &j);

public:
	Renderable();
	~Renderable();

	void SetMesh(std::string location);
	void SetEffect();
	void Render();
};