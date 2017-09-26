#pragma once
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_vec4.hpp>

class PointLight
{
public:
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;	
	
	float range;

	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;

	PointLight();
	~PointLight();
};

