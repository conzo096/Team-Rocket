#pragma once
#include "Entity.h"

class Material : public Component
{
protected:
	void from_json(const nlohmann::json &j);

public:
	Material();
	~Material();

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float shininess;

	void Render();
};
