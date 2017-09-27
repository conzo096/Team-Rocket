#pragma once
#include "Entity.h"
#include <glm/detail/type_vec4.hpp>

class Material : public Component
{
public:
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float shininess;

	Material();
	~Material();

	void from_json(const json& j) override;
	void Render() override;
};
