#pragma once
#include "Entity.h"
#include "Model.h"

class Renderable : public Component
{
protected:
	Model *model;
	void from_json(const nlohmann::json &j);

public:
	Renderable();
	~Renderable();

	void SetModel();
	void Render();
};